#include "boid.h"
#include <cmath>




Boid::Boid(int x, int y, float vx, float vy, int w, int h) {
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;

	this->max_speed = 3;
	this->min_speed = 2;

	this->w = w;
	this->h = h;

	this->separation = 2;
	this->vision_radius = 20;

	this->avoidance_factor = 0.05;
	this->alignment_factor = 0.005;
	this->cohesion_factor = 0.005;
	this->turn_factor = 1.0;

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
		int distance_squared = (abs(this->x - boid.x) * abs(this->x - boid.x) + abs(this->y - boid.y) * abs(this->y - boid.y));
		if (distance_squared < this->separation * this->separation) {
			colliding_boids.push_back(boid);
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
		avg_x += boid.vx;
		avg_y += boid.vy;
	}
	avg_x /= local_boids.size();
	avg_y /= local_boids.size();

	this->vx += (avg_x - this->x)*cohesion_factor;
	this->vy += (avg_y - this->y)*cohesion_factor;

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
		SDL_Rect boid_rect{boid.x -1, boid.y -1, 2, 2};
		SDL_RenderFillRect(this->renderer, &boid_rect);
		SDL_RenderDrawPoint(this->renderer, boid.x, boid.y);
	}

	//reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);
	return true;
}

bool Screen::render_screen() {
	SDL_RenderCopy(this->renderer, this->texture, nullptr, nullptr);
	return true;
}

bool Screen::add_boid(Boid boid) {
	this->boids.push_back(boid);
	return true;
}