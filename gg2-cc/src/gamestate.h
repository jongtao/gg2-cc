#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <allegro5/allegro5.h>
#include <iostream>

#include "constants.h"
#include "input.h"
#include "gamedata.h"
#include "render.h"
#include "audio.h"

using namespace std;



enum States
{
	PRETITLE_SCREEN,
	TITLE_SCREEN,
	PLATFORM_SCREEN
}; // enum States




class PreTitle
{
private:
	ALLEGRO_TIMER **timer;
	Audio *audio_manager;
	Render *render_manager;
	Input *inputs;


public:
	PreTitle(ALLEGRO_TIMER **timer, Audio *audio_manager,
		Render *render_manager, Input *inputs);

}; // PreTitle



class Title
{
private:
	bool begun;
	ALLEGRO_TIMER **timer;
	Audio *audio_manager;
	Render *render_manager;
	Input *inputs;


public:
	Title(ALLEGRO_TIMER **timer, Audio *audio_manager,
		Render *render_manager, Input *inputs);
	void init();
	int update();
	void draw();

}; // Title



class Platform
{
private:
	bool begun;
	ALLEGRO_TIMER **timer;
	Gamedata *gamedata;
	Audio *audio_manager;
	Render *render_manager;
	Input *inputs;



public:
	Platform(ALLEGRO_TIMER **timer, Gamedata *gamedata, Audio *audio_manager,
		Render *render_manager, Input *inputs);
	void init();
	int update();
	void draw();

}; // Platform


#endif
