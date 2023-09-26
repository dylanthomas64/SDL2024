#pragma once
#include <SDL.h>
#include <vector>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Test {
public:
	int a;
	Test();
	~Test();
};

struct Point {
	int x;
	int y;
};

class Floor {
public:
	int yPos;
	SDL_Point a{ 0 };
	SDL_Point b{ 0 };

	Floor();
	~Floor();
};

void render_floor(SDL_Renderer* renderer, Floor& floor);

struct Velocity {
	int x;
	int y;
};

class RectangleObject {
public:
	double v;
	SDL_Rect rect;
	Point origin;
	SDL_Color color;

	RectangleObject();
	RectangleObject(SDL_Rect);
	~RectangleObject();
};


void draw_filled_rectangle(SDL_Renderer* renderer, SDL_Rect outline_rect, SDL_Color color);


void paint(SDL_Renderer* renderer, int n, int t);

/*
void rectangle_with_velocity(SDL_Renderer* renderer, SDL_Rect& rect, Velocity v, int n);
*/

void clear_screen(SDL_Renderer* renderer);

std::vector<SDL_Rect> create_rectangles(int n, int size);

std::vector<RectangleObject> create_rectangle_objects(int n, int size);







void process(SDL_Renderer* renderer, std::vector<RectangleObject>& vec_rec, Uint32 delta_time, Floor &floor);