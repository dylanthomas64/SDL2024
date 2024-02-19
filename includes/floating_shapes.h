#ifndef FLOATING_SHAPES_H
#define FLOATING_SHAPES_H

#include <SDL.h>
#include <vector>


//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 900;

struct Float_Point {
	double x;
	double y;
};

class Floor {
public:
	int yPos;
	SDL_Point a{ 0 };
	SDL_Point b{ 0 };

	Floor();
	~Floor();
};

//general border class
class Border {
public:
	int yPos;
	std::vector<SDL_Point> points;

	Border(int yPos);
	~Border();
};

void render_border(SDL_Renderer* renderer, Border const& border);


void render_floor(SDL_Renderer* renderer, Floor const& floor);

struct Velocity {
	int x;
	int y;
};

class RectangleObject {
public:
	double y_velocity;
	double x_velocity;
	bool in_motion;
	SDL_Rect rect;
	Float_Point origin;
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

std::vector<RectangleObject> create_rectangle_objects(int n);







int process(SDL_Renderer* renderer, std::vector<RectangleObject>& vec_rec, Floor const& floor, int prev);

void render_all(SDL_Renderer* renderer, std::vector<RectangleObject> vec_rec, Floor floor);

//void render_background(SDL_Renderer* renderer);


//void render_reflection(SDL_Renderer* renderer, std::vector<RectangleObject> const vec_rec, Floor floor);


//void test();

#endif