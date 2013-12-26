// INIT
//
// Sets up allegro structs for use and cleans up at shutdown.

#ifndef INIT_H
#define INIT_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

void init(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	ALLEGRO_DISPLAY **display, bool *done);

void shutdown(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	ALLEGRO_DISPLAY **display);

void abort_game(const char *message);

#endif
