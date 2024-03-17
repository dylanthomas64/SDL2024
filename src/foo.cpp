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

bool LTexture::draw_coloured_fractal() {

	//boxed area c(r1, i1) to c(r2, i2)
	//get axis
	int r1 = -2;
	int r2 = 1;
	int i1 = -1;
	int i2 = 1;
	if (r1 > r2 || i1 > i2) {
		std::cout << "Error: Invalid area coordinates (top left to bottom right)";
	}

	int r_range = abs(r1) + abs(r2);
	int i_range = abs(i1) + abs(i2);

	double increment = 0.001;

	double boundary = 100.0;
	//number of iterations
	int iterations = 20;

	int middle_x = (mWidth / 2);
	int middle_y = mHeight / 2;



	// range of axis values
	int n_axis = 2.0;
	int scale = middle_x / n_axis;
	// scale = mWidth / 6


	if (this->mTexture == nullptr) {
		std::cout << "Error: texture is a nullptr\n";
		return false;
	}

	// set texture to render target
	SDL_SetRenderTarget(this->renderer, this->mTexture);

	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);






	

	std::pair<double, double> z0{ 0.0, 0.0 };
	std::pair<double, double> c{ 0.0, 0.0 };

	std::pair<double, double> z1 = z0;
	for (double r = r1; r <= r2; r += increment) {
		c.first = r;
		for (double i = i1; i <= i2; i += increment) {
			c.second = i;
			//std::cout << "(" << c.first << ", " << c.second << ")\t";

			// reset z to 0
			z1 = z0;

			long n = 0;
			// iterate
			for (double i = 0; i < iterations; i++) {
				//draw exact points as they iterate
				//SDL_RenderDrawPoint(this->renderer, origin_x + (z1.first * scale), origin_y + (z1.second * scale));
				if (z1.first * z1.first + z1.second * z1.second > boundary*boundary) {
					//colour realtive to n;
					break;
				}
				z1 = compute_next(z1, c);
				n++;
			}
			//double distance = complex_abs(z1);
			if (z1.first * z1.first + z1.second * z1.second <= 2*2) {
				//std::cout << distance << "\t";
				// set draw colour to red
				SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderDrawPoint(this->renderer, middle_x + (c.first * scale), middle_y + (c.second * scale));
			}
			/*
			else if (distance >= boundary){
				//SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//SDL_RenderDrawPoint(this->renderer, origin_x + (c.first * scale), origin_y + (c.second * scale));
			}
			*/
			else {
				int pixel_intensity = (255 * n/iterations);
				//std::cout << distance << "\t";
				SDL_SetRenderDrawColor(this->renderer, 0x00, pixel_intensity, pixel_intensity*0.6, 0xFF);
				SDL_RenderDrawPoint(this->renderer, middle_x + (c.first * scale), middle_y + (c.second * scale));
			}
			
		}

	}

	// draw axis
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < mHeight; i += 4)
	{
		SDL_RenderDrawPoint(this->renderer, middle_x, i);
	}
	for (int i = 0; i < mWidth; i += 4)
	{
		SDL_RenderDrawPoint(this->renderer, i, middle_y);
	}

	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//x - axis
	SDL_RenderDrawLine(this->renderer, middle_x + (-1 * scale), middle_y - (0.1 * scale), middle_x + (-1 * scale), middle_y + (0.1 * scale));
	SDL_RenderDrawLine(this->renderer, middle_x + (1 * scale), middle_y - (0.1 * scale), middle_x + (1 * scale), middle_y + (0.1 * scale));

	//y - axis
	SDL_RenderDrawLine(this->renderer, middle_x + (-0.1 * scale), middle_y + (-1 * scale), middle_x + (0.1 * scale), middle_y - (1 * scale));
	SDL_RenderDrawLine(this->renderer, middle_x + (-0.1 * scale), middle_y + (1 * scale), middle_x + (0.1 * scale), middle_y + (1 * scale));



	// reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);

	std::cout << "fractal created\n";
}

bool LTexture::draw_coloured_moveable_fractal(std::pair<double, double> top_left_coord, double range) {
	this->r = top_left_coord.first;
	this->i = top_left_coord.second;
	
	//boxed area c(r1, i1) to c(r2, i2)
	//get axis
	//x min
	int r_min = this->r;
	int r_max = r_min + range;
	//y max
	int i_max = this->i;
	int i_min = i_max - range;
	
	if (r_min > r_max || i_min > i_max) {
		std::cout << "Error: Invalid area coordinates (top left to bottom right)";
	}

	double increment = 0.0005;

	double boundary = 100.0;
	//number of iterations
	int iterations = 50;

	// pixels per x value
	int scale = mWidth / range;

	this->scale = scale;



	if (this->mTexture == nullptr) {
		std::cout << "Error: texture is a nullptr\n";
		return false;
	}

	// set texture to render target
	SDL_SetRenderTarget(this->renderer, this->mTexture);

	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);








	std::pair<double, double> z0{ 0.0, 0.0 };
	std::pair<double, double> c{ 0.0, 0.0 };

	std::pair<double, double> z1 = z0;
	for (double r = r_min; r <= r_max; r += increment) {
		c.first = r;
		for (double i = i_min; i <= i_max; i += increment) {
			c.second = i;
			//std::cout << "(" << c.first << ", " << c.second << ")\t";

			// reset z to 0
			z1 = z0;

			long n = 0;
			// iterate
			for (double i = 0; i < iterations; i++) {
				//draw exact points as they iterate
				//SDL_RenderDrawPoint(this->renderer, origin_x + (z1.first * scale), origin_y + (z1.second * scale));
				if (z1.first * z1.first + z1.second * z1.second > boundary * boundary) {
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
				SDL_RenderDrawPoint(this->renderer, (c.first - r_min) * scale, abs(c.second - i_max) * scale);
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
				SDL_RenderDrawPoint(this->renderer, (c.first - r_min) * scale, abs(c.second - i_max) * scale);
			}

		}

	}

	

	// reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);

	std::cout << "fractal created\n";
}
bool LTexture::draw_fractal(double range) {

	int r_min = this->r;
	int r_max = r_min + range;
	//y max
	int i_max = this->i;
	int i_min = i_max - range;

	if (r_min > r_max || i_min > i_max) {
		std::cout << "Error: Invalid area coordinates (top left to bottom right)";
	}

	double increment = 0.001;

	double boundary = 100.0;
	//number of iterations
	int iterations = 20;

	// pixels per x value
	int scale = mWidth / range;

	this->scale = scale;



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
	for (double r = r_min; r <= r_max; r += increment) {
		c.first = r;
		for (double i = i_min; i <= i_max; i += increment) {
			c.second = i;
			//std::cout << "(" << c.first << ", " << c.second << ")\t";

			// reset z to 0
			z1 = z0;

			long n = 0;
			// iterate
			for (double i = 0; i < iterations; i++) {
				//draw exact points as they iterate
				//SDL_RenderDrawPoint(this->renderer, origin_x + (z1.first * scale), origin_y + (z1.second * scale));
				if (z1.first * z1.first + z1.second * z1.second > boundary * boundary) {
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
				SDL_RenderDrawPoint(this->renderer, (c.first - r_min) * scale, abs(c.second - i_max) * scale);
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
				SDL_RenderDrawPoint(this->renderer, (c.first - r_min) * scale, abs(c.second - i_max) * scale);
			}

		}

	}



	// reset render target
	SDL_SetRenderTarget(this->renderer, nullptr);

	std::cout << "fractal created\n";
}

bool LTexture::draw_from_pixel_coord(std::pair<double, double> top_left_pixel_coord, double range) {

	this->r += (top_left_pixel_coord.first / this->scale);
	this->i -= (top_left_pixel_coord.second / this->scale);
	draw_fractal(2.0);

	return true;
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
