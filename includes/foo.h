#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "lib.h"
#include <cmath>

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromSurface(SDL_Renderer* renderer, ArtSurface *image_class);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = nullptr);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	SDL_Texture* getRaw();

	bool create_blank_texture(int width, int height, SDL_Renderer* renderer);
	bool draw_fractal();
	bool draw_coloured_fractal();
	bool draw_coloured_moveable_fractal(std::pair<double, double> top_left_global_coord, double range);
	bool draw_from_pixel_coord(std::pair<double, double> top_left_pixel_coord, double range);
	bool draw_fractal(double range);


private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	SDL_Renderer* renderer;

	//Image dimensions
	int mWidth;
	int mHeight;

	//fractal top left global coord
	double r;
	double i;
	//pixels per value
	double scale;
};

std::pair<double, double> compute_next(std::pair<double, double> z, std::pair<double, double> c);

double complex_abs(std::pair<double, double> c);
