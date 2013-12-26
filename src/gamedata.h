// Gamedata Class
//
// Stores Game data and other information (entities, maps, etc.)


#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "allegro5/allegro5.h"
#include <vector>
#include <list>
#include <cstring>

#include "mapdata.h"
#include "entity.h"
#include "audio.h"
#include "render.h"

class Entity;


class Gamedata
{
private:
	int counter;
	int tick;
	
	MapData *currentMap;
	std::list<Entity*> entities;
	
	Audio* audio_manager;
	Render* render_manager;
	Input* inputs;

public:
	Gamedata(Audio* audio_manager, Render* render_manager, Input* inputs,
		const char *filename);
	~Gamedata();
	void load_map(const char* mapName);
	void update();
	void add_player(int character, bool isLocal);
	void start_round();

	friend Entity;

}; // class Gamedata

#endif
