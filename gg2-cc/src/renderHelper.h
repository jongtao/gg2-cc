#ifndef RENDERHELPER_H
#define RENDERHELPER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "render.h"

class Render;



class RenderUnit
{
private:
	int type, pic;
	int r, g, b, t;
	float x, y, dx, dy, xscale, yscale, angle;
	bool relativeToMap;
public:
	RenderUnit(int type, int pic, int r, int g, int b, int t, float x, float y,
		float dx, float dy, float xscale, float yscale, float angle,
		bool relativeToMap);

	friend Render;

}; // class RenderUnit




class RenderPackage
{
private:
	ALLEGRO_BITMAP *bitmap;
	int center_x, center_y;
	
	friend Render;
public:
	RenderPackage();
}; // class RenderPackage


#endif
