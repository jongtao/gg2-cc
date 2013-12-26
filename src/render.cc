#include "render.h"



Render::Render(ALLEGRO_DISPLAY **display):
	px(0), py(0), width(GAME_WIDTH), height(GAME_HEIGHT), display(display)
{
	const char *files[MAP_BMP + 1] =
	{
		"characters/scoutreds/0.png",
		"ingameelements/crosshairs/0.png",
		"gameelements/menubackgrounds/0.png",
		"MAP PLACE HOLDER"
	};

	const int centers[(MAP_BMP + 1) * 2] =
	{
		//32,		40,
		0,		0,
		10,		10,
		150, 	150,
		0,		0
	};
	

	//bitmaps = new ALLEGRO_BITMAP*[MAP_BMP + 1];
	bitmaps = new RenderPackage[MAP_BMP + 1];
	
	ALLEGRO_PATH *exe_path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(exe_path, "resources"); // specific folder

	int error_exists = 0;

	for(int i = MAP_BMP - 1; i >= 0; i--) // minus one to skip map
	{
		printf("NUM:%d %s\n", i, files[i]);
		error_exists += resolve_load(&(bitmaps[i].bitmap), &exe_path, files[i]);

		bitmaps[i].center_x = centers[2 * i];
		bitmaps[i].center_y = centers[2 * i + 1];
	} // for i
	
	al_destroy_path(exe_path);

	if(error_exists)
	{
		for(int i = MAP_BMP - 1; i >=0; i--) // cleanly deconstruct on failure
			if(bitmaps[i].bitmap)
				al_destroy_bitmap(bitmaps[i].bitmap);

		if(bitmaps)
			free(bitmaps);

		abort_game("Could not find all the video resources!");
	} // if error_exists
} // Render()



Render::~Render()
{
	for(int i = MAP_BMP; i >=0; i--)
		if(bitmaps[i].bitmap)
			al_destroy_bitmap(bitmaps[i].bitmap);

	if(bitmaps)
		free(bitmaps);
} // ~Render()



void Render::set_display_size(int w, int h)
{
	disp_w = w;
	disp_h = h;
} // set_display_size()



void Render::load_map(const char *filename)
{
	ALLEGRO_PATH *exe_path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(exe_path, "resources/maps"); // specific folder

	if(resolve_load(&(bitmaps[MAP_BMP].bitmap), &exe_path, filename))
	{
		al_destroy_bitmap(bitmaps[MAP_BMP].bitmap);
	}

} // load_map()



void Render::draw_title()
{
	int w = al_get_bitmap_width(bitmaps[TITLE_BMP].bitmap);
	al_draw_bitmap(bitmaps[TITLE_BMP].bitmap, (disp_w - w) / 2, 0, 0);
} // draw_title()



void Render::draw_ingame(const Input *inputs)
{
	if(!width && !height) 
	{
		width = al_get_bitmap_width(bitmaps[MAP_BMP].bitmap);
		height = al_get_bitmap_height(bitmaps[MAP_BMP].bitmap);
	} // if width and height are both 0

	al_draw_scaled_bitmap(bitmaps[MAP_BMP].bitmap, 0, 0, width,
		height, disp_w/2 - px, disp_h/2 - py,  width * 6, height * 6, 0);

	draw_queue();

} // draw_ingame()



int Render::resolve_load(ALLEGRO_BITMAP **image, ALLEGRO_PATH **exe_path,
	const char *filename)
{
	const char *proper_path;

	al_set_path_filename(*exe_path, filename);

	if(*exe_path)
		proper_path = al_path_cstr(*exe_path, ALLEGRO_NATIVE_PATH_SEP);
	
	if(!(*image = al_load_bitmap(proper_path))) 
	{
		fprintf(stderr, "[Render Error] %s not found!\n", proper_path);
		return 1;
	} // if image load failed (NULL)

	return 0;
} // resolve_load(image)



void Render::queue(int type, int pic, int r, int g, int b, int t, float x,
	float y, float dx, float dy, float xscale, float yscale, float angle,
	bool relativeToMap)
{
	RenderUnit newUnit(type, pic, r, g, b, t, x, y, dx, dy, xscale, yscale,
		 angle, relativeToMap);
	renderQueue.push_back(newUnit);
} // queue()



void Render::draw_queue()
{
	vector<RenderUnit>::iterator qIterate;
	for(qIterate = renderQueue.begin(); qIterate != renderQueue.end();
		qIterate++)
	{
		//printf("%d: %d,%d\n", qIterate->pic, (int)qIterate->dx, (int)qIterate->dy);
		draw_now(qIterate->pic, qIterate->r, qIterate->g, qIterate->b,
			qIterate->t, qIterate->x, qIterate->y, qIterate->dx, qIterate->dy,
			qIterate->xscale, qIterate->yscale, qIterate->angle,
			qIterate->relativeToMap);
	} // for entIterate players

	
	renderQueue.clear();
} // draw_queue()




void Render::clear_queue()
{
	renderQueue.clear();
} // clear_queue()




void Render::draw_now(int pic, int r, int g, int b, int t, float x, float y,
	float dx, float dy, float xscale, float yscale, float angle,
	bool relativeToMap)
{
	dx = disp_w/2 - bitmaps[pic].center_x + dx;
	dy = disp_h/2 - bitmaps[pic].center_y + dy;

	if(relativeToMap)
	{
		dx -= px;
		dy -= py;
	} // relative to local

	al_draw_tinted_scaled_rotated_bitmap(bitmaps[pic].bitmap,
		al_map_rgba_f(r, g, b, t), x, y, dx, dy, xscale, yscale, angle, 0);

} // draw_now()



void Render::set_relative_pos(int x, int y)
{
	this->px = x;
	this->py = y;
} // set_relative_pos()



void Render::set_cursor(int pic)
{
	currentCursor = pic;
	al_hide_mouse_cursor(*display);
} // set_cursor()



void Render::reset_cursor()
{
	al_show_mouse_cursor(*display);
} // reset cursor()



void Render::draw_cursor(int x, int y)
{
	al_draw_bitmap(bitmaps[currentCursor].bitmap,
		x - bitmaps[currentCursor].center_x,
		y - bitmaps[currentCursor].center_y, 0);
} // draw_cursor
