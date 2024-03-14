#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "lib.h"

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

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	SDL_Renderer* renderer;

	//Image dimensions
	int mWidth;
	int mHeight;
};

