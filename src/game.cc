#include "game.h"

Game::Game(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	Audio *audio, Render *render):
	gamestate_mode(TITLE_SCREEN), event_queue(event_queue), timer(timer),
	gamedata(NULL), audio_manager(audio), render_manager(render)

{
	//gamedata = new Gamedata;
	pretitle_obj = new PreTitle(timer, audio, render, &inputs);
	title_obj = new Title(timer, audio, render, &inputs);
	platform_obj = new Platform(timer, gamedata, audio, render, &inputs);

} // Game()



Game::~Game()
{
	if(gamedata)
		delete gamedata;
	if(pretitle_obj)
		delete pretitle_obj;
	if(title_obj)
		delete title_obj;
	if(platform_obj)	
		delete platform_obj;
} // ~Game()



void Game::update()
{
	int last_mode = gamestate_mode;

	do
	{
		last_mode = gamestate_mode;

		if(gamestate_mode == TITLE_SCREEN)
			gamestate_mode = title_obj->update();
		else
			if(gamestate_mode == PLATFORM_SCREEN)
				gamestate_mode = platform_obj->update();
	} while(last_mode != gamestate_mode);

} // update()



void Game::draw()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	switch(gamestate_mode)
	{
		case PRETITLE_SCREEN:
			break;
		case TITLE_SCREEN:
			title_obj->draw();
			break;
		case PLATFORM_SCREEN:
			platform_obj->draw();
			break;
		default:
			break;
	}; // case(gamestate_mode)

	al_flip_display();
} // draw()



