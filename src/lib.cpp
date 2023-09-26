#include "lib.h"
#include <vector>
#include <random>
#include <iostream>


std::random_device rd;  // a seed source for the random number engine
std::mt19937 gen(rd());
std::uniform_int_distribution<> ud(0, SCREEN_WIDTH);
std::uniform_int_distribution<> cd(0, 0xFF);
std::uniform_int_distribution<> xv(-30, 30);
std::uniform_int_distribution<> size(4, 25);





Floor::Floor() {
	int yPos{ 0 };
	SDL_Point a{ 0 };
	SDL_Point b{ 0 };
	int floor = SCREEN_HEIGHT - 100;

	//set member values
	this->yPos = floor;
	this->a = SDL_Point{ 0, floor };
	this->b = SDL_Point{ SCREEN_WIDTH, floor };
	std::cout << "floor object constructed @ " << floor;
}
Floor::~Floor() = default;


//general border class
Border::Border(int yPos) : yPos(yPos) {
	std::vector<SDL_Point> v {};
	v.emplace_back(SDL_Point{ 0, yPos });
	v.emplace_back(SDL_Point{ SCREEN_WIDTH, yPos });
	this->points = v;

}

Border::~Border() = default;


RectangleObject::RectangleObject(SDL_Rect rect) : in_motion(true), rect(rect), x_velocity(0.0), y_velocity(0.0) {
	this->origin = Float_Point{
		static_cast<double>(rect.x),
		static_cast<double>(rect.y),
	};
	Uint8 r = cd(gen);
	Uint8 g = cd(gen);
	Uint8 b = cd(gen);
	Uint8 a = 0xFF;
	SDL_Color color{r, g, b, a};
	this->color = color;
};
RectangleObject::RectangleObject() : in_motion(true), x_velocity(0.0), y_velocity(0.0) {
	
	SDL_Rect rect{0, 0, 0, 0};
	this->rect = rect;
	this->origin = Float_Point{ 0.0, 0.0};

	this->color = SDL_Color{};
	this->color.r = 0x00;
	this->color.g = 0x00;
	this->color.r = 0x00;
	this->color.g = 0xFF;
	
}

RectangleObject::~RectangleObject() = default;

void draw_filled_rectangle(SDL_Renderer* renderer, SDL_Rect outline_rect, SDL_Color color) {

	// draw outline
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &outline_rect);

	// fill shape

	outline_rect.w -= 2;
	outline_rect.h -= 2;
	++outline_rect.x;
	++outline_rect.y;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); //#26E600
	SDL_RenderFillRect(renderer, &outline_rect);
}


 void clear_screen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}
 
 
 
 
 
 
void paint(SDL_Renderer* renderer, int n, int t) {
	//main loop
	for (int i = 0; i < n; i++) {
		SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);

		std::vector<SDL_Point> v;
		//std::vector<SDL_Rect> rv;

		for (int i = 0; i < 100; i++) {
			v.emplace_back(SDL_Point{ rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT });
			//rv.emplace_back(SDL_Rect{ rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 10, 10 });
		}

		//SDL_RenderDrawPoints(renderer, v.data(), v.size());
		SDL_RenderDrawLines(renderer, v.data(), v.size());
		//SDL_RenderDrawRects(renderer, rv.data(), rv.size());


		SDL_RenderPresent(renderer);
		SDL_Delay(t);
	}
}

void rectangle_with_velocity(SDL_Renderer* renderer, SDL_Rect& rect, Velocity v, int n) {
	int alpha = 0xFF;
	int alpha_step = 0xFF / n;
	for (int i = 0; i < n; i++) {
		//SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, alpha);
		SDL_RenderFillRect(renderer, &rect);
		rect.x += v.x;
		rect.y += v.y;
		alpha -= alpha_step;
		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	}

}

void render_border(SDL_Renderer* renderer, Border const& border) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLines(renderer, border.points.data(), border.points.size());
}

void render_floor(SDL_Renderer* renderer, Floor &floor) {
	std::vector<SDL_Point> v;
	
	v.emplace_back(floor.a);
	v.emplace_back(floor.b);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLines(renderer, v.data(), v.size());
	//std::cout << "floor rendered" << std::endl;
}

std::vector<SDL_Rect> create_rectangles(int n, int size) {
	std::vector<SDL_Rect> rects{};
	for (int i = n; i < n; i++) {
		rects.emplace_back(SDL_Rect{ rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, size, size });
	}
	return rects;
}

std::vector<RectangleObject> create_rectangle_objects(int n) {
	

	std::vector<RectangleObject> rects{};
	//std::cout << "\ncreating" << n << std::endl;
	for (int i = 0; i < n; i++) {
		int w = size(gen);
		std::cout << n << std::endl;
		SDL_Rect rect = { ud(gen) % SCREEN_WIDTH, ud(gen) % SCREEN_HEIGHT, w, w};
		//std::cout << "rect created\n";
		RectangleObject ro = RectangleObject(rect);
		//randomise x velocities
		ro.x_velocity = xv(gen);
		ro.y_velocity = xv(gen);
		//std::cout << "obj initialised from rectobj constructor";
		rects.emplace_back(ro);
	}
	return rects;
}


int process(SDL_Renderer* renderer, std::vector<RectangleObject>& vec_rec, Floor &floor, int previous_call_ticks) {
	
	int start_time = SDL_GetTicks();

	
	for (int i = 0; i < vec_rec.size(); i++) {


		if (vec_rec.at(i).in_motion) {

			// floor collision
			if (vec_rec.at(i).origin.y + vec_rec.at(i).rect.h >= floor.yPos) {
				//vec_rec.erase(vec_rec.begin() + i);
				//std::cout << "erased ";
				vec_rec.at(i).y_velocity *= -0.65;
				//vec_rec.at(i).v = -0.4 * vec_rec.at(i).v;
			}

			//ceiling collision
			if (vec_rec.at(i).origin.y <= 0) {
				vec_rec.at(i).y_velocity *= -0.7;
			}

			//wall collision
			if (vec_rec.at(i).origin.x <= 0 || vec_rec.at(i).origin.x + vec_rec.at(i).rect.w >= SCREEN_WIDTH) {
				vec_rec.at(i).x_velocity *= -0.7;
			}



			// get delta t in seconds
			double delta_t = static_cast<double>(start_time - previous_call_ticks) / 1000;
			//draw_filled_rectangle(renderer, vec_rec.at(i).rect, vec_rec.at(i).color);
			// v = u + at
			vec_rec.at(i).y_velocity += 9.81 * delta_t;
			// s1 = s0 + vt
			double new_y = vec_rec.at(i).origin.y + (vec_rec.at(i).y_velocity) * delta_t;
			vec_rec.at(i).origin.y = new_y;

			double new_x = vec_rec.at(i).origin.x + (vec_rec.at(i).x_velocity) * delta_t;
			vec_rec.at(i).origin.x = new_x;
		}


		
	}
	return start_time;
}

void render_all(SDL_Renderer* renderer, std::vector<RectangleObject> vec_rec, Floor floor) {
	
	clear_screen(renderer);
	render_floor(renderer, floor);
	for (RectangleObject obj : vec_rec) {
		// if yPos > floor: render at floor; else: render at yPos
		obj.rect.y = (obj.origin.y > floor.yPos) ? floor.yPos + obj.rect.h : static_cast<int>(obj.origin.y);
		obj.rect.x = obj.origin.x;
		draw_filled_rectangle(renderer, obj.rect, obj.color);
	}

	
	SDL_RenderPresent(renderer);
}