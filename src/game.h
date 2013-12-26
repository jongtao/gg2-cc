// Game Class
//
// Handles game logic


#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro5.h>
#include <iostream>

#include "input.h"
#include "gamedata.h"
#include "render.h"
#include "audio.h"
#include "gamestate.h"

using namespace std;




class Game
{
private:
	int gamestate_mode;

	ALLEGRO_EVENT_QUEUE **event_queue;
	ALLEGRO_TIMER **timer;
	Gamedata *gamedata;
	Audio *audio_manager;
	Render *render_manager;
	
	PreTitle *pretitle_obj;
	Title *title_obj;
	Platform *platform_obj;

public:
	Input inputs;

	Game(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer, Audio *audio,
		Render *render);
	~Game();
	void update();
	void draw();

}; // class Game



#endif
