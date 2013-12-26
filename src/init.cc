# include "init.h"

void init(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	ALLEGRO_DISPLAY **display, bool *done)
{
	if (!al_init())                                // Initialize Allegro
		abort_game("Failed to initialize allegro");

	if(!al_init_image_addon())                     // Initialize Image
		abort_game("Failed to initialize image engine");
									  
	if (!al_install_keyboard())                    // Install Keyboard
		abort_game("Failed to install keyboard");
	
	if(!al_install_mouse())                        // Install Mouse
		abort_game("Failed to install mouse");

	if (!(*timer = al_create_timer (TIMESTEP)))		 // Game tick at 60Hz
	  abort_game("Failed to create timer");

	if(!al_init_acodec_addon())                    // Initialize Audio Codecs
		abort_game("Failed to initilize audio codecs");

	if(!al_install_audio())                        // Initialize Audio
		abort_game("Failed to initialize audio engine");
	
	if(!al_reserve_samples(32))                    // Reserve audio mixer
		abort_game("Failed to reserve samples");
	
		// Display set to fixed window
	al_set_new_display_flags(ALLEGRO_RESIZABLE);    
	al_inhibit_screensaver(true);

		// Display sized 800 x 400
	if (!(*display = al_create_display(GAME_WIDTH, GAME_HEIGHT)))
		abort_game("Failed to create display");

																																			 
	if (!(*event_queue = al_create_event_queue())) // Initialize Event Queue
		abort_game("Failed to create event queue");
	
	// Register Timer and Keyboard to Event Queue
	al_register_event_source(*event_queue, al_get_keyboard_event_source());
	al_register_event_source(*event_queue, al_get_mouse_event_source());
	al_register_event_source(*event_queue, al_get_timer_event_source(*timer));
	al_register_event_source(*event_queue, al_get_display_event_source(*display));

	al_set_window_title(*display, "Gang Garrison 2 C++");
	*done = false; // Initialized Succesfully!
} // init()



void shutdown(ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer,
	ALLEGRO_DISPLAY **display)
{
  if (*timer)
    al_destroy_timer(*timer);
		 
  if (*display)
	  al_destroy_display(*display);
														  
	if (*event_queue)
	  al_destroy_event_queue(*event_queue);

	printf("Stopped cleanly.\n");
	
} // shutdown()



void abort_game(const char *message)
{
	fprintf(stderr, "[SEVERE] %s\nAborting...\n", message);
	exit(1);
	// quick_exit();
} // abort_game()

