#include "lib.h"
#include <vector>
#include <random>
#include <iostream>


Test::Test() {
	a = 20;
}
Test::~Test() = default;


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



std::random_device rd;  // a seed source for the random number engine
std::mt19937 gen(rd());
std::uniform_int_distribution<> ud(0, SCREEN_WIDTH);
std::uniform_int_distribution<> cd(0, 0xFF);


RectangleObject::RectangleObject(SDL_Rect rect) : rect(rect), v(0.0) {
	this->origin = Point{
		rect.x,
		rect.y,
	};
	Uint8 r = cd(gen);
	Uint8 g = cd(gen);
	Uint8 b = cd(gen);
	Uint8 a = 0xFF;
	SDL_Color color{r, g, b, a};
	this->color = color;
};
RectangleObject::RectangleObject() : v(0.0) {
	
	SDL_Rect rect{0, 0, 0, 0};
	this->rect = rect;
	this->origin = Point{ rect.x, rect.y };

	this->color = SDL_Color{};
	this->color.r = 0x00;
	this->color.g = 0x00;
	this->color.r = 0x00;
	this->color.g = 0xFF;
	
}

RectangleObject::~RectangleObject() = default;

void draw_filled_rectangle(SDL_Renderer* renderer, SDL_Rect outline_rect, SDL_Color color) {

	// draw outline
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

std::vector<RectangleObject> create_rectangle_objects(int n, int size) {
	

	std::vector<RectangleObject> rects{};
	//std::cout << "\ncreating" << n << std::endl;
	for (int i = 0; i < n; i++) {
		std::cout << n << std::endl;
		SDL_Rect rect = { ud(gen) % SCREEN_WIDTH, ud(gen) % SCREEN_HEIGHT, size, size };
		//std::cout << "rect created\n";
		RectangleObject ro = RectangleObject(rect);
		//std::cout << "obj initialised from rectobj constructor";
		rects.emplace_back(ro);
	}
	return rects;
}


void process(SDL_Renderer* renderer, std::vector<RectangleObject>& vec_rec, Uint32 delta_time, Floor &floor) {
	clear_screen(renderer);

	//render_floor(renderer, floor);
	for (int i = 0; i < vec_rec.size(); i++) {
		if (vec_rec.at(i).rect.y > SCREEN_HEIGHT) {
			vec_rec.erase(vec_rec.begin() + i);
			std::cout << "erased";
		}
		else {
			draw_filled_rectangle(renderer, vec_rec.at(i).rect, vec_rec.at(i).color);
			// v = u + at
			vec_rec.at(i).v += 9.81 * delta_time;
			// s1 = s0 + vt
			double new_y = vec_rec.at(i).origin.y + (vec_rec.at(i).v) * delta_time;
			vec_rec.at(i).rect.y = new_y;

			//if (new_y > floor.)

		}
		
	}
	SDL_RenderPresent(renderer);
	//std::cout << "fulshed\n";
}