// block

#include <allegro5/allegro5.h>
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "constants.h"
#include "init.h"
#include "game.h"
#include "render.h"
#include "audio.h"

#define MAX_FRAMESKIP 5
  
void game_loop(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	ALLEGRO_DISPLAY **display, bool *done)
{
	al_start_timer(*timer);
	int skips = 0;
	double initialTime = 0, updateDelta = 0, renderDelta = 0;;
	ALLEGRO_EVENT event;

	Render render(display);
	Audio audio;
	Game engine(event_queue,timer, &audio, &render);

	int width = al_get_display_width(*display);
	int height = al_get_display_height(*display);
	render.set_display_size(width, height);


	printf("\nClose Window or press Escape to quit.\n");

	bool redraw = true; // True means we need to redraw screen
	while (!*done)
	{
		al_wait_for_event(*event_queue, &event);

		initialTime = al_get_time(); // get start time

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			engine.update(); 		 // Update engine
		} // if timer ticked (update phase)
		else
		{
			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // close window clicked
				break;

			engine.inputs.gather(&event);
		} // else collect inputs


		updateDelta = al_get_time() - initialTime;


		if(updateDelta + renderDelta > TIMESTEP * 1.03) // 3% threshold
		{
			if(skips < MAX_FRAMESKIP)
			{
				skips++;
				cout << renderDelta << " skips: "<< skips << " " <<redraw << 
					"  Frameskip! =============================" << endl;
				redraw = false;
			}
			else // reset frameskip
			{
				skips = 0;
				redraw = true;
			} // max frameskip
		} // predict if update + render time is too long

		if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			al_acknowledge_resize(*display);
			render.set_display_size(al_get_display_width(*display),
				al_get_display_height(*display));
		} // if window resized

		

 		if (redraw && al_is_event_queue_empty(*event_queue))
		{
			engine.draw();
			redraw = false;
		} // if engine updated and even queye is empty (output phase)

		renderDelta = al_get_time() - initialTime - updateDelta;

  } // while
} // game_loop()
		  
int main(int argc, char* argv[])
{
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_DISPLAY* display;

	bool done; // game keeps running when false

	init(&event_queue, &timer, &display, &done);
	game_loop(&event_queue, &timer, &display, &done);
	shutdown(&event_queue, &timer, &display);

	return 0;
} // main()
