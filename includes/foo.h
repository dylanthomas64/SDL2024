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
	bool draw_from_pixel_coord(std::pair<double, double> top_left_pixel_coord, double range);
	bool draw_offset_fractal(std::pair<double, double> c_offset, long double range=1.0);
	std::pair<double, double> pixel_to_global_complex(std::pair<int, int> pixel_coord);

	void set_fractal_detail(int iterations, long long int boundary);




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

	std::pair<double, double> fractal_centre{};
	// abs(r1 - r0);
	double range;
	//pixels per value
	double scale;
	

	int iterations;
	long long int boundary;
};

std::pair<double, double> compute_next(std::pair<double, double> z, std::pair<double, double> c);

double complex_abs(std::pair<double, double> c);

