// Input Class
//
// Detects and stores inputs

#ifndef INPUT_H
#define INPUT_H
#include <allegro5/allegro5.h>
#include <iostream>
#include <cmath>
#include "constants.h"

class Input
{
private:
public:
	bool up, down, left, right, space;
	bool leftmouse, middlemouse, rightmouse;
	double aim_angle;
	int aim_x, aim_y; // raw
	int wild_down;
	//int controls[8]; // will read control from file to map keys

	Input();
	void gather(const ALLEGRO_EVENT *event);
};

#endif
