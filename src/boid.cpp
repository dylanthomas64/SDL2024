#include "boid.h"
#include <cmath>
#include <algorithm>




Boid::Boid(int x, int y, float vx, float vy, int w, int h) {
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;

	this->max_speed = 3;
	this->min_speed = 2;

	this->colour.r = rand() % 255;
	this->colour.g = rand() % 255;
	this->colour.b = rand() % 255;
	this->colour.a = 255;

	this->w = w;
	this->h = h;

	this->separation = 2;
	this->vision_radius = 20;

	this->avoidance_factor = 0.05;
	this->alignment_factor = 0.05;
	this->cohesion_factor = 0.005;
	this->turn_factor = 0.2;

}

Boid::~Boid() {
}


void Boid::react_to_local_boids(std::vector<Boid> all_boids) {
	// find all local boids within vision radius
	std::vector<Boid> local_boids{};
	for (Boid boid : all_boids) {
		int distance_squared = (abs(this->x - boid.x)* abs(this->x - boid.x) + abs(this->y - boid.y)*abs(this->y - boid.y));
		if (distance_squared <= this->vision_radius * this->vision_radius) {
			// don't include self
			if (distance_squared != 0) {
				local_boids.push_back(boid);
			}
			
		}
	}

	if (local_boids.size() == 0) {
		return;
	}
	//find all local boids within separation radius
	std::vector<Boid> colliding_boids{};
	for (Boid boid : local_boids) {
		int dx = this->x - boid.x;
		int dy = this->y - boid.y;
		int distance_squared = (dx * dx) + (dy * dy);
		if (distance_squared <= this->separation * this->separation) {
			//check if in field of view
			double theta;
			double local_theta;
			if (this->vx == 0) {
				theta = 0;
				if (this->vy > 0) theta = 180;
			} else {
				theta = atan(this->vy / this->vx);
			}
			if (dx == 0) {
				local_theta = 0;
				if (dy > 0) theta = 180;
			}
			else {
				local_theta = atan(dy / dx);
			}
			
			if (abs(theta - local_theta) <= this->fov) {
				colliding_boids.push_back(boid);
			}
		}
	}

	//separation
	int dx = 0;
	int dy = 0;
	for (Boid boid : colliding_boids) {
		dx += this->x - boid.x;
		dy += this->y - boid.y;
	}
	this->vx += dx * avoidance_factor;
	this->vy += dy * avoidance_factor;



	//remove colliding boids from local boids ??

	//alignment


	
	double avg_vx = 0;
	double avg_vy = 0;
	for (Boid boid : local_boids) {
		avg_vx += boid.vx;
		avg_vy += boid.vy;
	}
	avg_vx /= local_boids.size();
	avg_vy /= local_boids.size();
	this->vx += avg_vx * alignment_factor;
	this->vy += avg_vy * alignment_factor;
	

	//cohesion
	int avg_x = 0;
	int avg_y = 0;
	for (Boid boid : local_boids) {
		avg_x += boid.x;
		avg_y += boid.y;
	}
	avg_x /= local_boids.size();
	avg_y /= local_boids.size();

	this->vx += (this->x - avg_x)*cohesion_factor;
	this->vy += (this->y - avg_y)*cohesion_factor;


	
	// window edge detection
	if (this->x < 0) this->vx += this->turn_factor;
	if (this->x > this->w) this->vx -= this->turn_factor;
	if (this->y > this->h) this->vy -= this->turn_factor;
	if (this->y < 0) this->vy += this->turn_factor;

	double curr_speed = std::sqrt((this->vx * this->vx) + (this->vy * this->vy));
	if (curr_speed > this->max_speed) {
		this->vx = (this->vx / curr_speed) * this->max_speed;
		this->vy = (this->vy / curr_speed) * this->min_speed;
	}
	else if (curr_speed < this->min_speed) {
		this->vx = (this->vx / curr_speed) * this->min_speed;
		this->vy = (this->vy / curr_speed) * this->min_speed;
	}

}
void Boid::update_position() {
	this->x += this->vx;
	this->y += this->vy;
}



Screen::Screen(SDL_Renderer* renderer, int w, int h) {
	this->renderer = renderer;
	this->screen_width = w;
	this->screen_height = h;

	//create blank texture
	this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->screen_width, this->screen_height);
}

Screen::~Screen() {
	this->renderer = nullptr;
	this->texture = nullptr;


	this->screen_width = 0;
	this->screen_height = 0;
}

void Screen::step() {
	// for each boid react to all other boids
	for (Boid& boid : this->boids) {
		boid.react_to_local_boids(boids);
	}
	// update each boids position
	for (Boid& boid : this->boids) {
		boid.update_position();
	}
}

bool Screen::render_texture() {


	// set texture to render target
	SDL_SetRenderTarget(this->renderer, this->texture);

	// clear texture
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);

	
	

	// set boid colour
	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
	//set boid fillrect
	

	for (Boid boid : this->boids) {
		SDL_Rect boid_rect{boid.x -2, boid.y -2, 4, 4};
		// set boid colour
		SDL_SetRenderDrawColor(this->renderer, boid.colour.r, boid.colour.g, boid.colour.b, 0xFF);
		SDL_RenderFillRect(this->renderer, &boid_rect);
		SDL_RenderDrawPoint(this->renderer, boid.x, boid.y);
	}

	//reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);
	return true;
}

bool Screen::render_screen() {
	//SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(this->renderer, this->texture, nullptr, nullptr);
	return true;
}

bool Screen::add_boid(Boid boid) {
	this->boids.push_back(boid);
	return true;
}

void run_boids() {
	

	// constructs a screen class with a blank texture
	Screen screen{ gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT };


	// add 30 boids
	for (int i = 0; i < 700; i++) {
		float r0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		if (rand() % 2 == 0) {
			r0 *= -1;
		}
		if (rand() % 2 == 0) {
			r1 *= -1;
		}

		screen.add_boid(Boid{ rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, r0, r1, SCREEN_WIDTH, SCREEN_HEIGHT });
	}


	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//If mouse event happened

			if (e.type == SDL_MOUSEBUTTONDOWN) {

				int x, y;
				SDL_GetMouseState(&x, &y);
				/*
				if (e.button.button == SDL_BUTTON_LEFT) {
					std::cout << "stepping\n";
					screen.step();
					screen.render_texture();
				}

				else if (e.button.button == SDL_BUTTON_RIGHT) {

				}
				*/

			}
		}




		//render texture to screen
		screen.render_screen();
		//screen.step();

		//Update screen
		SDL_RenderPresent(gRenderer);
		screen.step();
		screen.render_texture();
	}
}
