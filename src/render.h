// Render Class
//
// Renders all game elements

#ifndef RENDER_H
#define RENDER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <cstdio>
#include <vector>

#include "constants.h"
#include "input.h"
#include "init.h"
#include "renderHelper.h"

using namespace std;


class RenderPackage;
class RenderUnit;



enum File_names
{
	SCOUT0_BMP, CURSOR_BMP, TITLE_BMP, MAP_BMP
}; // enum File_names



class Render
{
private:
	int px, py; // location of perspective
	int width, height;
	int disp_w, disp_h;
	int currentCursor;
	RenderPackage *bitmaps;

	ALLEGRO_DISPLAY **display;
	vector<RenderUnit> renderQueue;

public:
	Render(ALLEGRO_DISPLAY **display);
	~Render();

	void set_display_size(int w, int h);
	
	void load_map(const char *filename);

	void draw_title();

	void draw_ingame(const Input *inputs);

	int resolve_load(ALLEGRO_BITMAP **image, ALLEGRO_PATH **exe_path,
		const char *filename);

	void queue(int type, int pic, int r, int g, int b, int t, float x, float y,
		float dx, float dy, float xscale, float yscale, float angle,
		bool relativeToMap);

	void draw_queue();

	void clear_queue();

	void draw_now(int pic, int r, int g, int b, int t, float x, float y, float dx,
		float dy, float xscale, float yscale, float angle, bool relativeToMap);

	void set_relative_pos(int x, int y);

	void set_cursor(int pic);

	void reset_cursor();

	void draw_cursor(int x, int y);

}; // class render




#endif
