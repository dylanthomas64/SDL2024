#include <SDL.h>
#include <iostream>
#include <vector>
#include "lib.h"
#include <random>


// pixels * 2 = metres
const int SCALE = 2;


int main(int argc, char* arg[])
{

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	//SDL_RenderSetScale(renderer, 2, 2);
	clear_screen(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);




	std::vector<RectangleObject> vec_rec = create_rectangle_objects(600);
	Floor floor;
	// 80fps => T = 1/80
	const int fps = 70;
	int total_frames = 10'000;

	int previous_call_ticks_since_start = SDL_GetTicks();
	for (int i = 0; i < total_frames; i++) {
		previous_call_ticks_since_start = process(renderer, vec_rec, floor, previous_call_ticks_since_start);
		render_all(renderer, vec_rec, floor);
		//convert fps to Time interval in ms
		SDL_Delay(1000/fps);

	}
	
	return 0;
}

