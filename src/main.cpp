/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "boid.h"

//Screen dimension constants
const int SCREEN_WIDTH = 560;
const int SCREEN_HEIGHT = 560;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;




// MAIN -------------------------------------- //

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{

		// constructs a screen class with a blank texture
		Screen screen{gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT};

		
		// add 30 boids
		for (int i = 0; i < 900; i++) {
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

		//screen.step();

		screen.render_texture();

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

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			//render texture to screen
			screen.render_screen();
			//screen.step();

			//Update screen
			SDL_RenderPresent(gRenderer);
			screen.step();
			screen.render_texture();
			}
	}

	//Free resources and close SDL
	close();

	return 0;
}


// MAIN -------------------------------------- //


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


void close()
{
	//Free loaded images
	//image_class.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}