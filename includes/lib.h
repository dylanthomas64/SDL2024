#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>



class ArtSurface
{
public:
	//Initializes variables
	ArtSurface();

	//Deallocates memory
	~ArtSurface();

	//Loads image at specified path
	bool loadFromFile(std::string path);


	//Renders surface at given point
	void render(int x, int y, SDL_Rect* clip = nullptr);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//creates a 2d vector to store raw pixel data as SDL_Colours
	void create_vec();

	//rebuilds surface from vector
	void updateSurface();
	void blur();
	void shrink(int factor);

	void save_copy(std::string fn);

	SDL_Surface* get_raw();


private:
	//The actual hardware surface
	SDL_Surface* realSurface;

	std::vector<std::vector<SDL_Colour>> colour_vec;

	//Image dimensions
	int surface_w;
	int surface_h;
	int vec_w;
	int vec_h;
};


/*
void do_colour_stuff(SDL_Surface* surface);


void do_colour_stuff_2d(SDL_Surface* surface);


*/
//void shrink(SDL_Surface* surface, int factor);

std::vector<std::vector<SDL_Colour>> blur(std::vector<std::vector<SDL_Colour>> vec2, int w, int h);

