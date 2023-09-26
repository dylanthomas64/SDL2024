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


	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> ud(0, SCREEN_WIDTH);
	//for (int i = 0; i < 25; i++) std::cout << ud(gen) << " \n";




	//std::cout << "creating vector";
	std::vector<RectangleObject> vec_rec = create_rectangle_objects(6, 10);
	/*
	for (int i = 0; i < 10000; i++) {
		//process(renderer, vec_rec, SDL_GetTicks()/1000, floor);
		SDL_Delay(10);
	}*/

	//paint(renderer, 10, 100);

	
	//render_floor(renderer, floor);
	
	//SDL_RenderDrawPoints(renderer, floor.points.data(), floor.points.size());
	//SDL_RenderDrawLines(renderer, floor.points.data(), floor.points.size());
	//std::cout << "floor drawn" << std::endl;

	

	Floor floor;
	// 80fps => T = 1/80
	int fps = 80;
	int total_frames = 100;

	for (int i = 0; i < total_frames; i++) {
		process(renderer, vec_rec, SDL_GetTicks() / 1000, floor);
		SDL_RenderPresent(renderer);
		//convert fps to Time interval in ms
		SDL_Delay(1000/fps);

	}
	
	



	
	

	/*
	for (int i = 0; i < vec_rec.size(); i++) {
		std::cout << "rendering @ " << vec_rec.at(i).origin.x << ", " << vec_rec.at(i).origin.y << std::endl;
		draw_filled_rectangle(renderer, vec_rec.at(i).rect);
	}
	SDL_RenderPresent(renderer);
	*/

	//std::cout << "proccessing\n";

	/*
	for (int i = 0; i < 100; i++) {
		process(renderer, vec_rec, SDL_GetTicks()/1000);
		SDL_Delay(15);
		std::cout << "ok!\n";

	};

	/*
	double delta_t{ 0.2 }; // seconds
	double v{0};
	double y{ 0 };
	double t{0};
	while (y <= SCREEN_HEIGHT) {
		SDL_Rect outline_rect{ SCREEN_WIDTH / 2, y, 5, 5 };
		draw_filled_rectangle(renderer, outline_rect);
		v = v + (9.81 * delta_t);
		y += (v * delta_t);
		
		SDL_RenderPresent(renderer);
		SDL_Delay(delta_t * 1000);
	}
	*/

	SDL_Delay(5000);
	
	return 0;
}

