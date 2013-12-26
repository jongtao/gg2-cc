#include "entity.h"

Entity::Entity(int i, Audio* audio_manager, Render* render_manager,
	Input* inputs, MapData *currentMap, bool isLocal, Gamedata *gamedata):
	id_num(i), type(0), x(0), y(0), vx(0), vy(0), ax(0), ay(0),
	isLocal(isLocal), isAlive(false),
	audio_manager(audio_manager), render_manager(render_manager), inputs(inputs),
	currentMap(currentMap), gamedata(gamedata)
{
} // Entity()



Entity::~Entity()
{
	if(!isLocal)
		delete inputs;
} // ~Entity()



int Entity::fetch_id()
{
	return id_num;
} // fetch_id()



Spectator::Spectator(int i, Audio* audio_manager, Render* render_manager,
	Input* inputs, MapData *currentMap, bool isLocal, Gamedata *gamedata):
	Entity(i, audio_manager, render_manager, inputs, currentMap, isLocal, gamedata)
{
}



void Spectator::spawn()
{
	int temp_x, temp_y;

	currentMap->get_rand_spawn(1, (unsigned)(al_get_time()*1000000), &temp_x,
		&temp_y); // team is 1(red) for testing
	x = (float)temp_x;
	y = (float)temp_y;

	isAlive = true;
}



void Spectator::update(int tick)
{
	x += vx;
	y += vy;

	//cout << currentMap->fetch_wallmask((unsigned int)x, (unsigned int)y);
	if(inputs->up && currentMap->fetch_wallmask(x, y-1))
		y-=3;
	if(inputs->down && currentMap->fetch_wallmask(x, y+1))
		y+=3;
	if(inputs->left && currentMap->fetch_wallmask(x-1, y))
		x-=3;
	if(inputs->right && currentMap->fetch_wallmask(x+1, y))
		x+=3;
	
	
	if(!isAlive)
		return;
		
	render_manager->queue(type, SCOUT0_BMP, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 0, !isLocal);

	if(isLocal)
		render_manager->set_relative_pos(x, y);
}



RedScout::RedScout(int i, Audio* audio_manager, Render* render_manager,
	Input* inputs, MapData *currentMap, bool isLocal, Gamedata *gamedata):
	Entity(i, audio_manager, render_manager, inputs, currentMap, isLocal, gamedata),
	width(3), height(9), acceleration(20 * TIMESTEP), max_speed(200 * TIMESTEP)
{
}



void RedScout::spawn()
{
	int temp_x, temp_y;

	currentMap->get_rand_spawn(1, (unsigned)(al_get_time()*1000000), &temp_x,
		&temp_y); // team is 1(red) for testing
	x = (float)temp_x;
	y = (float)temp_y;

	isAlive = true;

}



void RedScout::movement()
{
} // movement



void RedScout::update(int tick)
{
	// save data before change
	float tx = x;
	float ty = y;
	float tvx = vx;
	float tvy = vy;

	if(isAlive)
	{
		
		//cout << currentMap->fetch_wallmask((unsigned int)x, (unsigned int)y);
		if(inputs->up && physics.on_ground(tx, ty, width, currentMap))
			tvy -= acceleration + 4;
		if(inputs->down)
			tvy += acceleration;
		if(inputs->left && (-tvx <= max_speed))
			tvx -= acceleration;
		if(inputs->right && (tvx <= max_speed))
			tvx += acceleration;

		physics.gravity(&tvy);
		physics.friction(&tvx, &tvy);

		physics.check_wall_col(&tvx, &tvy, tx, ty, width, height, currentMap);

		x = tx + tvx;
		y = ty + tvy;

		if(tvx || tvy)
			printf("vx: %f, vy: %f\n", tvx, tvy);

		vx = tvx;
		vy = tvy;
	
		if(isLocal) // draw at center
			render_manager->queue(type, SCOUT0_BMP, 1, 1, 1, 1, 0, 0,
				0, 0, 1, 1, 0, !isLocal);
		else // draw relative
			render_manager->queue(type, SCOUT0_BMP, 1, 1, 1, 1, 0, 0,
				x, y, 1, 1, 0, !isLocal);
	} // if alive
	
	if(isLocal)
		render_manager->set_relative_pos(x, y);
} // Redscout::update()
