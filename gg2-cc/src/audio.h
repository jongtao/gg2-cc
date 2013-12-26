// Audio class
//
// Contains audio and allows other classes to play them

#ifndef AUDIO_H
#define AUDIO_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <cstdio>
#include "init.h"


enum Song_names
{
	GUILES_SG, TITLE_SG
}; // enum Song_files



enum Sound_names
{
	COIN_SD
}; // enum Sound_files


class Audio
{
private:
	ALLEGRO_SAMPLE **songs;
	ALLEGRO_SAMPLE **sounds;
	ALLEGRO_SAMPLE_INSTANCE *song_instance;
	int is_playing;

public:

	Audio();
	~Audio();

	void play_song(int song_num);
	void stop_song();
	int get_is_playing();
	void play_sound(int sound_num, float pan);
	int resolve_load(ALLEGRO_SAMPLE **sample, ALLEGRO_PATH **exe_path,
		const char *filename);

}; // class Audio

#endif
