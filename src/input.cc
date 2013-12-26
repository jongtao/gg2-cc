#include "input.h"

Input::Input():
	up(0), down(0), left(0), right(0), space(0),
	leftmouse(0), middlemouse(0), rightmouse(0),
	aim_angle(0), aim_x(GAME_WIDTH/2), aim_y(GAME_HEIGHT/2), wild_down(0)
{
} // Input constructor

void Input::gather(const ALLEGRO_EVENT *event)
{
	if(event->type == ALLEGRO_EVENT_MOUSE_AXES ||
		event->type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
	{
		aim_angle = atan2(-(event->mouse.y - (GAME_HEIGHT / 2)),
			event->mouse.x - (GAME_WIDTH / 2));

		aim_x = event->mouse.x;
		aim_y = event->mouse.y;

		//std::cout << aim_angle << std::endl;
	} // if mouse

	if(event->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		std::cout << "CLICK DOWN " << event->mouse.button << std::endl;

		switch(event->mouse.button)
		{
			case 1:
				leftmouse = true;
				break;
			case 2:
				rightmouse = true;
				break;
			case 3:
				middlemouse = true;
				break;
		} // switch mouse button
	} // if mouse button down

	if(event->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		std::cout << "CLICK UP " << event->mouse.button << std::endl;
		
		switch(event->mouse.button)
		{
			case 1:
				leftmouse = false;
				break;
			case 2:
				rightmouse = false;
				break;
			case 3:
				middlemouse = false;
				break;
		} // switch mouse button
	} // if mouse button up

	if(event->type == ALLEGRO_EVENT_KEY_DOWN)
	{
		std::cout << event->keyboard.keycode << std::endl;

		wild_down = event->keyboard.keycode;

		switch(event->keyboard.keycode)
		{
			case ALLEGRO_KEY_W:
				up = true;
				break;
			case ALLEGRO_KEY_A:
				left = true;
				break;
			case ALLEGRO_KEY_S:
				down = true;
				break;
			case ALLEGRO_KEY_D:
				right = true;
				break;
		} // switch keyboard keycode
	} // if key down

	if(event->type == ALLEGRO_EVENT_KEY_UP)
	{
		std::cout << event->keyboard.keycode << std::endl;

		wild_down = 0;

		switch(event->keyboard.keycode)
		{
			case ALLEGRO_KEY_W:
				up = false;
				break;
			case ALLEGRO_KEY_A:
				left = false;
				break;
			case ALLEGRO_KEY_S:
				down = false;
				break;
			case ALLEGRO_KEY_D:
				right = false;
				break;
		} // switch keyboard keycode
	} // if key down

} // gather()
