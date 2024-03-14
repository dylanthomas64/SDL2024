#include "lib.h"



ArtSurface::ArtSurface() {
	realSurface = nullptr;
	surface_w = 0;
	surface_h = 0;
	vec_w = 0;
	vec_h = 0;
}

ArtSurface::~ArtSurface() {
	//Free texture if it exists
	if (realSurface != nullptr)
	{
		SDL_FreeSurface(realSurface);
		realSurface = nullptr;
		//colour_vec.clear();
		surface_w = 0;
		surface_h = 0;
		vec_w = 0;
		vec_h = 0;
	}
}

bool ArtSurface::loadFromFile(std::string path) {
	realSurface = IMG_Load(path.c_str());
	if (realSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {

		std::cout << "surface loaded\n";
	}
	return (realSurface != nullptr);
}

void ArtSurface::updateSurface() {
	Uint8* raw_pixel_ptr = (Uint8*)realSurface->pixels;

	int w = colour_vec.at(1).size();
	int h = colour_vec.size();


	realSurface->h = w;
	realSurface->w = h;
	realSurface->pitch = w * 4;



	std::cout << "writing to surface...\n";
	//assign to surface
	int i = 0;
	for (std::vector<SDL_Colour> row : colour_vec) {
		for (SDL_Colour colour : row) {
			raw_pixel_ptr[i] = colour.r;
			++i;
			raw_pixel_ptr[i] = colour.g;
			++i;
			raw_pixel_ptr[i] = colour.b;
			++i;
			raw_pixel_ptr[i] = colour.a;
			++i;
		}
	}
	raw_pixel_ptr[++i] = NULL;
	std::cout << "DONE!\n";
}

void ArtSurface::create_vec() {
	//convert to vector

	Uint8* raw_pixel_ptr = (Uint8*)realSurface->pixels;
	int size = realSurface->w * realSurface->h;

	std::vector<std::vector<SDL_Colour>> temp_vec2d{};
	SDL_Colour temp_colour{};
	int i = 0;
	for (int y = 0; y < realSurface->h; y++) {
		std::vector<SDL_Colour> row{};
		for (int x = 0; x < realSurface->w; x++) {
			i = (4 * x) + (realSurface->pitch * y);
			temp_colour.r = static_cast<Uint8>(raw_pixel_ptr[i + 0]);
			temp_colour.g = static_cast<Uint8>(raw_pixel_ptr[i + 1]);
			temp_colour.b = static_cast<Uint8>(raw_pixel_ptr[i + 2]);
			temp_colour.a = static_cast<Uint8>(raw_pixel_ptr[i + 3]);
			//row.emplace_back(SDL_Colour {})
			row.push_back(temp_colour);
		}
		temp_vec2d.push_back(row);
	}
	
	colour_vec = temp_vec2d;
	std::cout << "vector created from surface\n";
}

void ArtSurface::blur() {
	std::cout << "blurring...\n";
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	SDL_Colour temp_colour{};
	std::vector<SDL_Colour> colours_to_blend{};
	std::vector<std::vector<SDL_Colour>> output_vector{};
	std::vector<SDL_Colour> output_row{};
	for (int y = 0; y < vec_h; y++) {
		for (int x = 0; x < vec_w; x++) {




			// get surrounding colours
			for (int i = -1; i < 2; i++) {
				if (y + i < 0 || y + i >= vec_h) continue;
				for (int j = -1; j < 2; j++) {
					if (x + j < 0 || x + j >= vec_w) continue;
					colours_to_blend.push_back(colour_vec.at(y + i).at(x + j));
				}
			}

			//add weight to average
			//colours_to_blend.push_back(vec2.at(y).at(x));
			//colours_to_blend.push_back(vec2.at(y).at(x));

			int size = colours_to_blend.size();
			for (int i = 0; i < size; i++) {
				r += colours_to_blend.at(i).r;
				g += colours_to_blend.at(i).g;
				b += colours_to_blend.at(i).b;
				a += colours_to_blend.at(i).a;
			}

			temp_colour.r = r / size;
			temp_colour.g = g / size;
			temp_colour.b = b / size;
			temp_colour.a = a / size;
			r = 0;
			g = 0;
			b = 0;
			b = 0;
			colours_to_blend.clear();
			output_row.push_back(temp_colour);


		}
		output_vector.push_back(output_row);
		output_row.clear();
		//std::cout << "row added...\n";
	}
	//std::cout << "DONE!...\n";
	colour_vec = output_vector;
}

void ArtSurface::save_copy() {
	std::string path = "output/img.png";
	if (IMG_SavePNG(realSurface, "output/image.png") < 0) {
		std::cout << "error saving file\n";
	}
}

SDL_Surface* ArtSurface::get_raw() {
	return realSurface;
}



std::vector<std::vector<SDL_Colour>> blur(std::vector<std::vector<SDL_Colour>> vec2, int w, int h) {
	std::cout << "blurring...\n";
	int r=0;
	int g=0;
	int b=0;
	int a=0;
	SDL_Colour temp_colour{};
	std::vector<SDL_Colour> colours_to_blend{};
	std::vector<std::vector<SDL_Colour>> output_vector{};
	std::vector<SDL_Colour> output_row{};
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			



			// get surrounding colours
			for (int i = -1; i < 2; i++) {
				if (y + i < 0 || y + i >= h) continue;
				for (int j = -1; j < 2; j++) {
					if (x + j < 0 || x + j >= w) continue;
					colours_to_blend.push_back(vec2.at(y+i).at(x+j));
				}
			}

			//add weight to average
			//colours_to_blend.push_back(vec2.at(y).at(x));
			//colours_to_blend.push_back(vec2.at(y).at(x));

			int size = colours_to_blend.size();
			for (int i = 0; i < size; i++) {
				r += colours_to_blend.at(i).r;
				g += colours_to_blend.at(i).g;
				b += colours_to_blend.at(i).b;
				a += colours_to_blend.at(i).a;
			}
			
			temp_colour.r = r / size;
			temp_colour.g = g / size;
			temp_colour.b = b / size;
			temp_colour.a = a / size;
			r = 0;
			g = 0;
			b = 0;
			b = 0;
			colours_to_blend.clear();
			output_row.push_back(temp_colour);
			

		}
		output_vector.push_back(output_row);
		output_row.clear();
		//std::cout << "row added...\n";
	}
	//std::cout << "DONE!...\n";
	return output_vector;
}





void shrink(SDL_Surface* surface, int factor) {
	std::cout << "shrinking...\n";


	Uint8* raw_pixel_ptr = (Uint8*)surface->pixels;
	int size = surface->w * surface->h;

	int new_w = surface->w / factor;
	int new_h = surface->h / factor;


	std::vector<std::vector<SDL_Colour>> colour_vec2d{};
	SDL_Colour temp_colour{};
	int i = 0;
	for (int y = 0; y < surface->h; y++) {
		std::vector<SDL_Colour> row{};
		for (int x = 0; x < surface->w; x++) {
			i = (4 * x) + (surface->pitch * y);
			temp_colour.r = raw_pixel_ptr[i + 0];
			temp_colour.g = raw_pixel_ptr[i + 1];
			temp_colour.b = raw_pixel_ptr[i + 2];
			temp_colour.a = raw_pixel_ptr[i + 3];
			//row.emplace_back(SDL_Colour {})
			row.push_back(temp_colour);
		}
		colour_vec2d.push_back(row);
	}

	std::cout << "created vector...\n";

	std::cout << "shrinking... ";
	std::vector<std::vector<SDL_Colour>> shrunk_vec{};
	for (int y = 0; y < surface->h; y+= factor) {
		std::vector<SDL_Colour> shrunk_row{};
		for (int x = 0; x < surface->w; x += factor) {
			shrunk_row.push_back(colour_vec2d.at(y).at(x));
		}
		shrunk_vec.push_back(shrunk_row);
	}
	std::cout << "DONE\n";
	std::cout << "(w, h): " << surface->w << ", " << surface->h << "\n(n_w, n_h)" << new_w << ", " << new_h << std::endl;
	surface->h = new_h;
	surface->w = new_w;
	surface->pitch = new_h * 4;



	std::cout << "writing to surface...\n";
	//assign to surface
	i = 0;
	for (std::vector<SDL_Colour> colour_vec : shrunk_vec) {
		for (SDL_Colour colour : colour_vec) {
			raw_pixel_ptr[i] = colour.r;
			++i;
			raw_pixel_ptr[i] = colour.g;
			++i;
			raw_pixel_ptr[i] = colour.b;
			++i;
			raw_pixel_ptr[i] = colour.a;
			++i;
		}
	}
	raw_pixel_ptr[++i] = NULL;
	std::cout << "DONE!\n";
}



/*


void do_colour_stuff(SDL_Surface* surface) {

	Uint8* raw_pixel_ptr = (Uint8*)surface->pixels;

	int size = surface->w * surface->h;


	//write contents to a colour vector
	std::vector<SDL_Colour> colour_vec{};
	SDL_Colour temp_colour{};
	for (int i = 0; i < size*4; i += 4) {
		temp_colour.r = raw_pixel_ptr[i+0];
		temp_colour.g = raw_pixel_ptr[i+1];
		temp_colour.b = raw_pixel_ptr[i+2];
		temp_colour.a = raw_pixel_ptr[i+3];
		colour_vec.push_back(temp_colour);
	}

	std::vector<std::vector<SDL_Colour>> colour_vec2d{};
	SDL_Colour temp_colour{};
	for (int y = 0; y < surface->h; y++) {
		std::vector<SDL_Colour> row{};
		for (int x = 0; x < surface->w; x++) {
			temp_colour.r = raw_pixel_ptr[x + 0];
			temp_colour.g = raw_pixel_ptr[x + 1];
			temp_colour.b = raw_pixel_ptr[x + 2];
			temp_colour.a = raw_pixel_ptr[x + 3];
			//row.emplace_back(SDL_Colour {})
			row.push_back(temp_colour);
		}
		colour_vec2d.push_back(row);
	}




	//wipe original surface
	//write colour vector to surface
	/*
	int i = -1;
	for (SDL_Colour colour : colour_vec) {
		raw_pixel_ptr[++i] = 0xFF;
		raw_pixel_ptr[++i] = 0xFF;
		raw_pixel_ptr[++i] = 0xFF;
		raw_pixel_ptr[++i] = 0xFF;
	}
	*/

	//write colour vector to surface
	/*
	int i = -1;
	for (SDL_Colour colour : colour_vec) {
		raw_pixel_ptr[++i] = colour.r;
		raw_pixel_ptr[++i] = colour.g;
		raw_pixel_ptr[++i] = colour.b;
		raw_pixel_ptr[++i] = colour.a;
	}


void do_colour_stuff_2d(SDL_Surface* surface) {

	auto colour_vec2d = surface_to_vec(surface);



	// do stuff

	auto blurred_vec = blur(colour_vec2d, surface->w, surface->h);
	std::cout << "original: " << colour_vec2d.at(0).size() << "\nblurred: " << blurred_vec.at(0).size() << std::endl;


	std::cout << "writing blurred image to surface... ";

	Uint8* raw_pixel_ptr = (Uint8*)surface->pixels;
	//assign to surface
	for (int y = 0; y < surface->h; y++) {
		for (int x = 0; x < surface->w; x++) {
			int i = (x*4) + (y * surface->pitch);
			raw_pixel_ptr[i] = blurred_vec.at(y).at(x).r;
			raw_pixel_ptr[i+1] = blurred_vec.at(y).at(x).g;
			raw_pixel_ptr[i+2] = blurred_vec.at(y).at(x).b;
			raw_pixel_ptr[i+3] = blurred_vec.at(y).at(x).a;
		}
	}
	std::cout << "DONE!";
}

*/