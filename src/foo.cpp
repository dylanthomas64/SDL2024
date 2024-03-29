#include "foo.h"
#include "lib.h"



LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	renderer = nullptr;

	//fractal
	fractal_centre = std::pair<double, double>{ 0.0, 0.0 };
	this->range = 3;
	this->scale = 0;
	this->iterations = 0;
	this->boundary = 0;
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

	//std::cout << "creating blank texture\n";
	// must have target access in order to render to texture
	this->mTexture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

	if (this->mTexture == nullptr) {
		std::cout << "Error: new blank texture is null: " << SDL_GetError() << std::endl;
	}

	//std::cout << "making texture the render target\n";
	//Make self render target
	SDL_SetRenderTarget(this->renderer, mTexture);

	//create blank pink
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);

	//std::cout << "render target cleared with pink\n";


	// make window the render target
	SDL_SetRenderTarget(this->renderer, nullptr);


	this->mWidth = width;
	this->mHeight = height;

	if (this->mTexture == nullptr) {
		std::cout << "texture is null.....";
	}

	return this->mTexture != nullptr;

}

bool LTexture::draw_from_pixel_coord(std::pair<double, double> pixel_coord, double zoom_factor) {

	int x = pixel_coord.first;
	int y = pixel_coord.second;

	int x_centre = mWidth / 2;
	int y_centre = mHeight / 2;

	// calculate change in pixels between centre of screen and cursor
	int dx = x - x_centre;
	int dy = y_centre - y;

	double r = this->fractal_centre.first + (dx / scale);
	double i = this->fractal_centre.second + (dy / scale);

	std::pair<double, double> complex_centre{ r, i };
	draw_offset_fractal(complex_centre, this->range/zoom_factor);

	return true;
}

bool LTexture::draw_offset_fractal(std::pair<double, double> complex_centre_coord, long double range) {

	this->fractal_centre = complex_centre_coord;
	this->range = range;
	// pixels per cartesian unit
	int scale = mWidth / this->range;
	this->scale = scale;


	double r = complex_centre_coord.first;
	double i = complex_centre_coord.second;

	// centre the fractal around point
	double rmin = r - (0.5 * this->range);
	double imax = i + (0.5 * this->range);
	double rmax = rmin + this->range;
	double imin = imax - this->range;



	
	

	

	double increment = this->range / mWidth;

	



	if (this->mTexture == nullptr) {
		std::cout << "Error: texture is a nullptr\n";
		return false;
	}

	// set texture to render target
	SDL_SetRenderTarget(this->renderer, this->mTexture);

	SDL_SetRenderDrawColor(this->renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderClear(this->renderer);








	std::pair<double, double> z0{ 0.0, 0.0 };
	std::pair<double, double> c{ 0.0, 0.0 };

	std::pair<double, double> z1 = z0;
	for (double r = rmin; r <= rmax; r += increment) {
		c.first = r;
		for (double i = imin; i <= imax; i += increment) {
			c.second = i;
			//std::cout << "(" << c.first << ", " << c.second << ")\t";

			// reset z to 0
			z1 = z0;

			long n = 0;
			// iterate
			for (double i = 0; i < this->iterations; i++) {
				//draw exact points as they iterate
				//SDL_RenderDrawPoint(this->renderer, origin_x + (z1.first * scale), origin_y + (z1.second * scale));
				if (z1.first * z1.first + z1.second * z1.second > this->boundary * this->boundary) {
					//colour realtive to n;
					break;
				}
				z1 = compute_next(z1, c);
				n++;
			}
			//double distance = complex_abs(z1);
			if (z1.first * z1.first + z1.second * z1.second <= 2 * 2) {
				//std::cout << distance << "\t";
				// set draw colour to red
				SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0x00, 0xFF);
				// x: xmin -> x0 = s
				// 
				// y: ymax -> y0 = s
				SDL_RenderDrawPoint(this->renderer, (c.first - rmin) * scale, abs(c.second - imax) * scale);
			}
			/*
			else if (distance >= boundary){
				//SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//SDL_RenderDrawPoint(this->renderer, origin_x + (c.first * scale), origin_y + (c.second * scale));
			}
			*/
			else {
				int pixel_intensity = (255 * n / iterations);
				//std::cout << distance << "\t";
				SDL_SetRenderDrawColor(this->renderer, 0x00, pixel_intensity, pixel_intensity * 0.6, 0xFF);
				SDL_RenderDrawPoint(this->renderer, (c.first - rmin) * scale, abs(c.second - imax) * scale);
			}

		}

	}



	// reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);

	std::cout << "\ncentre = (" << this->fractal_centre.first << ", " << this->fractal_centre.second << ")\t" << "range: " << range << std::endl;
}


double complex_abs(std::pair<double, double> c) {

	//return abs value of a complex number
	double root = std::sqrt(((c.first * c.first) + (c.second * c.second)));
	return std::abs(root);
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



void LTexture::set_fractal_detail(int iterations, long long int boundary) {
	this->iterations = iterations;
	this->boundary = boundary;
}