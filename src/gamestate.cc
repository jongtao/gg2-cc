#include "gamestate.h"


PreTitle::PreTitle(ALLEGRO_TIMER **timer,
	Audio *audio_manager, Render *render_manager, Input *inputs):
	timer(timer),audio_manager(audio_manager),
	render_manager(render_manager), inputs(inputs)
{
}



Title::Title(ALLEGRO_TIMER **timer,
	Audio *audio_manager, Render *render_manager, Input *inputs):
	begun(false), timer(timer),	audio_manager(audio_manager),
	render_manager(render_manager), inputs(inputs)
{
}



void Title::init()
{
	audio_manager->play_sound(1, 0);
	audio_manager->stop_song();
	audio_manager->play_song(TITLE_SG);
	render_manager->reset_cursor();
} // init()



int Title::update()
{
	if(begun == false)
	{
		begun = true;
		init();
	}

	if (inputs->wild_down == 67)
	{
		begun = false;
		inputs->wild_down = 0;
		return PLATFORM_SCREEN;
	}

 	return TITLE_SCREEN;
} // update()



void Title::draw()
{
	render_manager->draw_title();
	//render_manager->draw_now(CURSOR_BMP, 1, 1, 1, 1,
	//	-(float)inputs->aim_x, -(float)inputs->aim_y, -10, -10, 1, 1, 0);

} // draw()



Platform::Platform(ALLEGRO_TIMER **timer, Gamedata *gamedata,
	Audio *audio_manager, Render *render_manager, Input *inputs):
	begun(false), timer(timer), gamedata(gamedata),	audio_manager(audio_manager),
	render_manager(render_manager), inputs(inputs)
{
}



void Platform::init()
{
	audio_manager->play_sound(1, 0);
	audio_manager->stop_song();
	audio_manager->play_song(GUILES_SG);
	render_manager->set_cursor(CURSOR_BMP);

	if(gamedata)
	{
		delete gamedata;
		gamedata = NULL;
	}

	gamedata = new Gamedata(audio_manager, render_manager, inputs,
		"ctf_buddybase_v1.png");
	gamedata->start_round();
} // init()




int Platform::update()
{
	if(begun == false)
	{
		begun = true;
		init();
	}

	if (inputs->wild_down == 67) 
	{
		begun = false;
		inputs->wild_down = 0;
		return TITLE_SCREEN;
	} // if switch to Title


	gamedata->update();

	return PLATFORM_SCREEN;
}



void Platform::draw()
{
	if(begun == false) // debug
		printf("gamestate not initialized!");

	render_manager->draw_ingame(inputs);
	render_manager->draw_cursor(inputs->aim_x, inputs->aim_y);
	// Draw Cursor
	//render_manager->draw_now(CURSOR_BMP, 1, 1, 1, 1,
	//	-(float)inputs->aim_x, -(float)inputs->aim_y, -10, -10, 1, 1, 0);
}
