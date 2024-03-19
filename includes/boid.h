#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_image.h"

class Boid {
public:
	Boid(int x, int y, float vx, float vy, int w, int h);
	~Boid();
	//pixel coordinates
	float x{ 0 };
	float y{ 0 };

	int max_speed;
	int min_speed;

	float vx{ 0 };
	float vy{ 0 };
	int separation{ 5 };
	int vision_radius{ 20 };
	double fov = 0.5 * 3.14;

	double avoidance_factor{ 1 };
	double alignment_factor{ 1 };
	double cohesion_factor{ 1 };
	double turn_factor{ 1 };


	SDL_Colour colour;

	// screen dimensions
	int w;
	int h;



	void react_to_local_boids(std::vector<Boid> all_boids);
	void update_position();

};

class Screen {
public:
	Screen(SDL_Renderer* renderer, int w, int h);
	~Screen();
	void step();
	bool render_texture();
	bool render_screen();
	bool add_boid(Boid boid);

private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	int screen_width;
	int screen_height;

	std::vector<Boid> boids;
};



//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

extern SDL_Renderer* gRenderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// run program
void run_boids();