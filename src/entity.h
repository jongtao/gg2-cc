// Abstract Entity class
//
// Contains data for entity

#ifndef ENTITY_H
#define ENTITY_H

#include <cstdlib>
#include <vector>
#include <iostream> // debug

#include "audio.h"
#include "render.h"
#include "input.h"
#include "mapdata.h"
#include "constants.h"
#include "gamedata.h"
#include "physics.h"

class Gamedata;


class Entity
{
//private:
protected:
	int id_num, type;
	float x, y; // position
	float vx, vy; // velocity
	float ax, ay; // acceleration
	int top, bottom, left, right; // body box
	bool isLocal, isAlive;

	Audio* audio_manager;
	Render* render_manager;
	Input* inputs;
	MapData *currentMap;
	Physics physics;

	Gamedata *gamedata;

public:
	Entity(int i, Audio* audio_manager, Render* render_manager, Input* inputs,
		MapData *currentMap, bool isLocal, Gamedata *gamedata);
	~Entity();

	int fetch_id();
	virtual void spawn() = 0;
	virtual void update(int tick) = 0;

	friend class Gamestate;
}; // class Entity




class Spectator: public Entity
{
private:
public:
	Spectator(int i, Audio* audio_manager, Render* render_manager, Input* inputs,
		MapData *currentMap, bool isLocal, Gamedata *gamedata);

	virtual void spawn();
	virtual void update(int tick);
}; // class Spectator




class RedScout: public Entity
{
private:
	int width, height;
	float acceleration, max_speed;

	void movement();

public:
	RedScout(int i, Audio* audio_manager, Render* render_manager, Input* inputs,
		MapData *currentMap, bool isLocal, Gamedata *gamedata);

	virtual void spawn();
	virtual void update(int tick);

}; // class RedScout


#endif
