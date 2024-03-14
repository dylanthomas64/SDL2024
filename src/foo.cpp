#include "foo.h"
#include "lib.h"



LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	renderer = nullptr;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromSurface(SDL_Renderer* renderer, ArtSurface *image_class)
{
	// give ltexture pointer to renderer
	this->renderer = renderer;


	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = nullptr;

	//duplicate pointer data to a temp surface
	SDL_Surface* loadedSurface = nullptr;
	loadedSurface = image_class->get_raw();

	if (loadedSurface == nullptr)
	{
		printf("Error: surface is a nullptr\n");
	}
	else
	{

		


		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture. SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	this->mTexture = newTexture;


	return mTexture != NULL;


}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	//std::cout << "rendering texture\n";
	if (this->mTexture == nullptr) {
		std::cout << "texture is null\n";
	}
	else if (this->renderer == nullptr) {
		std::cout << "renderer is null\n";
	}
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(this->renderer, this->mTexture, nullptr, clip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

SDL_Texture* LTexture::getRaw()
{
	return mTexture;
}

bool LTexture::create_blank_texture(int width, int height, SDL_Renderer* renderer) {

	// give ltexture pointer to renderer
	this->renderer = renderer;

	if (this->mTexture != nullptr) {
		std::cout << "deleting old texture\n";
	}
	free();

	std::cout << "creating blank texture\n";
	// must have target access in order to render to texture
	this->mTexture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

	if (this->mTexture == nullptr) {
		std::cout << "Error: new blank texture is null: " << SDL_GetError() << std::endl;
	}

	std::cout << "making texture the render target\n";
	//Make self render target
	SDL_SetRenderTarget(this->renderer, mTexture);

	//create blank pink
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);

	std::cout << "render target cleared with pink\n";


	// make window the render target
	SDL_SetRenderTarget(this->renderer, nullptr);


	this->mWidth = width;
	this->mHeight = height;

	if (this->mTexture == nullptr) {
		std::cout << "texture is null.....";
	}

	return this->mTexture != nullptr;

}

bool LTexture::draw_fractal() {

	
	int origin_x = mWidth / 2;
	int origin_y = mHeight / 2;

	// range of axis values
	int n_axis = 2.0;
	int scale = origin_x / n_axis;
	// scale = mWidth / 6


	if (this->mTexture == nullptr) {
		std::cout << "Error: texture is a nullptr\n";
		return false;
	}

	// set texture to render target
	SDL_SetRenderTarget(this->renderer, this->mTexture);

	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);

	



	
	// set draw colour to red
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0x00, 0xFF);

	std::pair<double, double> z0{ 0.0, 0.0 };
	std::pair<double, double> c{ 0.0, 0.0 };

	std::pair<double, double> z1 = z0;
	for (double r = -2; r <= 1; r += 0.001) {
		c.first = r;
		for (double i = -1; i <= 1; i += 0.001) {
			c.second = i;
			//std::cout << "(" << c.first << ", " << c.second << ")\t";
			
			// reset z to 0
			z1 = z0;


			double boundary = 10.0;
			//number of iterations
			int n = 0;
			// iterate
			for (double i = 0; i < 30; i++) {
				//draw exact points as they iterate
				//SDL_RenderDrawPoint(this->renderer, origin_x + (z1.first * scale), origin_y + (z1.second * scale));
				z1 = compute_next(z1, c);
				n++;
			}
			if (complex_abs(z1) <= 2) {
				SDL_RenderDrawPoint(this->renderer, origin_x + (c.first * scale), origin_y + (c.second * scale));
			}

		}

	}
	
	// draw axis
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < mHeight; i += 4)
	{
		SDL_RenderDrawPoint(this->renderer, origin_x, i);
	}
	for (int i = 0; i < mWidth; i += 4)
	{
		SDL_RenderDrawPoint(this->renderer, i, origin_y);
	}

	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//x - axis
	SDL_RenderDrawLine(this->renderer, origin_x + (-1 * scale), origin_y - (0.1 * scale), origin_x + (-1 * scale), origin_y + (0.1 * scale));
	SDL_RenderDrawLine(this->renderer, origin_x + (1 * scale), origin_y - (0.1 * scale), origin_x + (1 * scale), origin_y + (0.1 * scale));

	//y - axis
	SDL_RenderDrawLine(this->renderer, origin_x + (-0.1 * scale), origin_y + (-1 * scale), origin_x + (0.1 * scale), origin_y - (1 * scale));
	SDL_RenderDrawLine(this->renderer, origin_x + (-0.1 * scale), origin_y + (1 * scale), origin_x + (0.1 * scale), origin_y + (1 * scale));



	// reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);
}

double complex_abs(std::pair<double, double> c) {

	//return abs value of a complex number
	return std::abs(std::sqrt(((c.first * c.first) + (c.second * c.second))));
}

std::pair<double, double> compute_next(std::pair<double, double> z, std::pair<double, double > c) {
	/*
	Z = Z ^ 2 + C
	Z^2 = (a + bi)^2 = (a + bi)(a +bi)
	= a^2 + 2abi -b^2
	= a^2 - b^2 + 2*a*b*i
	r = a^2 - b^2
	i = 2*a*b
	*/
	double r1 = z.first;
	double i1 = z.second;

	double r2 = (r1 * r1) - (i1 * i1);
	double i2 = 2.0 * r1 * i1;

	// make next complex number and add constants
	std::pair<double, double> z2{ r2 + c.first, i2 + c.second };

	return z2;
}