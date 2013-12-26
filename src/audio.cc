#include "audio.h"

Audio::Audio():song_instance(NULL), is_playing(0)
{
	const char *song_files[3] =
		{
			"song.ogg", "title.wav"
		};

	const char *sound_files[1] =
		{
			"coin.wav"
		};

	songs = (ALLEGRO_SAMPLE**)malloc((TITLE_SG + 1) * sizeof(ALLEGRO_SAMPLE*));
	sounds = (ALLEGRO_SAMPLE**)malloc((COIN_SD + 1) * sizeof(ALLEGRO_SAMPLE*));

	// Get path relative to exe
	ALLEGRO_PATH *exe_path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(exe_path, "resources"); // specific folder

	int error_exists = 0;

	for(int i = TITLE_SG; i >= 0; i--)
	{
		printf("SONG NUM:%d %s\n", i, song_files[i]);
		error_exists += resolve_load(&(songs[i]), &exe_path, song_files[i]);
	} // for i

	for(int i = COIN_SD; i >= 0; i--)
	{
		printf("SOUND NUM:%d %s\n", i, sound_files[i]);
		error_exists += resolve_load(&(sounds[i]), &exe_path, sound_files[i]);
	} // for i

	
	al_destroy_path(exe_path);

	if(error_exists)
	{
		for(int i = TITLE_SG; i >= 0; i--) // cleanly deconstruct on failure
			if(songs[i])
				al_destroy_sample(songs[i]);

		for(int i = COIN_SD; i >= 0; i--) // cleanly deconstruct on failure
			if(sounds[i])
				al_destroy_sample(sounds[i]);

		if(songs)
			free(songs);

		if(sounds)
			free(sounds);

		abort_game("Could not find all the audio resources!");
	} // if error exists
} // Audio()



Audio::~Audio()
{
	for(int i = TITLE_SG; i >= 0; i--) // cleanly deconstruct on failure
		if(songs[i])
			al_destroy_sample(songs[i]);

	for(int i = COIN_SD; i >= 0; i--) // cleanly deconstruct on failure
		if(sounds[i])
			al_destroy_sample(sounds[i]);

	if(songs)
		free(songs);

	if(sounds)
		free(sounds);
} // ~Audio()



void Audio::play_song(int song_num)
{
	if(is_playing == 1)
		return;

	ALLEGRO_SAMPLE *song;
	double gain;

	switch(song_num)
	{
		case GUILES_SG:
			song = songs[GUILES_SG];
			gain = 0.7;
			break;
		case TITLE_SG:
			song = songs[TITLE_SG];
			gain = 1.2;
			break;
		default:
			return;
			break;
	} // case(song_num)

	song_instance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(song_instance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(song_instance, gain);
	al_attach_sample_instance_to_mixer(song_instance, al_get_default_mixer());

	al_play_sample_instance(song_instance);

	printf("Playing Song_%d\n", song_num); // FIXME debug
	is_playing = 1;
} // play_song()



void Audio::stop_song()
{
	if(!is_playing)
		return;

	al_stop_sample_instance(song_instance);
	is_playing = 0;
} // stop_song()



int Audio::get_is_playing()
{
	return is_playing;
} // get_is_playing()



void Audio::play_sound(int sound_num, float pan)
{
	ALLEGRO_SAMPLE *sound;
	//double gain;
	switch(sound_num)
	{
		case 1:
		sound = sounds[COIN_SD];
		break;
		default:
			return;
			break;
	} // case(song_num)

	if(!(al_play_sample(sound, 1.6, pan, 1.0, ALLEGRO_PLAYMODE_ONCE, 0)))
		fprintf(stderr, "[Audio Error] Could not play sample %d!\n", sound_num);
} // play_sound()



int Audio::resolve_load(ALLEGRO_SAMPLE **sample, ALLEGRO_PATH **exe_path,
	const char *filename)
{
	const char *proper_path;

	al_set_path_filename(*exe_path, filename);

	if(*exe_path)
		proper_path = al_path_cstr(*exe_path, ALLEGRO_NATIVE_PATH_SEP);
	
	if(!(*sample = al_load_sample(proper_path))) 
	{
		fprintf(stderr, "[Audio Error] %s not found!\n", proper_path);
		return 1;
	} // if sample load failed (NULL)

	return 0;
} // resolve_load(sample)

