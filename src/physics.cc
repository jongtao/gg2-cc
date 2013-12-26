#include "physics.h"


Physics::Physics(): grav_a(GRAV_A)
{
} // Physics()



int Physics::on_ground(float x, float y, int width, MapData* currentMap)
{
	x = (int)(x / 6);
	y = (int)(y / 6);
	return !currentMap->fetch_wallmask(x, y + 1);
} // on_ground()



void Physics::friction(float *vx, float *vy)
{
	if(*vx > 0)
	{
		*vx -= (FRICTION * TIMESTEP);

		if(*vx < 0)
			*vx = 0;
	} // if positive velocity


	if(*vx < 0)
	{
		*vx += (FRICTION * TIMESTEP);

		if(*vx > 0)
			*vx = 0;
	} // if negative velocity


	if(*vy > 0)
	{
		*vy -= (FRICTION * TIMESTEP);

		if(*vy < 0)
			*vy = 0;
	} // if positive velocity


	if(*vy < 0)
	{
		*vy += (FRICTION * TIMESTEP);

		if(*vy > 0)
			*vy = 0;
	} // if negative velocity

} // ground_friction()


/*
void Physics::friction(float *vx, float x, float y, MapData* currentMap)
{
	if(*vx == 0)
		return;

	//int temp_vx = *vx;
	int temp_x = (int)(x / 6);
	int temp_y = (int)(y / 6);
	bool onGround = !currentMap->fetch_wallmask(temp_x, temp_y + 1);

	if(*vx > 0)
	{
		if(onGround)
		{
			*vx -= (FRICTION * TIMESTEP);

			if(*vx < 0)
				*vx = 0;

		} // if ground is underneath
	} // if positive velocity


	if(*vx < 0)
	{
		if(onGround)
		{
			*vx += (FRICTION * TIMESTEP);

			if(*vx > 0)
				*vx = 0;

		} // if ground is underneath
	} // if negative velocity

} // ground_friction()
*/


void Physics::check_x_wall(float *vx, float *vy, float x, float y, int width,
	int height,	MapData* currentMap)
{
	if(*vx == 0)
		return;

	int temp_vx = 0;
	int temp_x, temp_y;

	if(*vx > 0) // moving right
	{
		temp_vx = (int)ceil(*vx);
		// rounding position down to scale down game's position matrix to
		// the wallmask's (yeah truncate yadayada)
		temp_y = (int)(y / 6);

		// simulate steps
		for(int i = 1; i <= temp_vx; i++)
		{
			temp_x = (int)floor((x + i) / 6);
			// check if next step is in the wall
			if(!currentMap->fetch_wallmask(temp_x, temp_y)) 
			{
				// get as close to the wall instead of going through it
				*vx = i - 1;
				return;
			} // if it is wall
		} // for i
	} // vx positive


	if(*vx < 0) // moving left
	{
		temp_vx = (int)floor(*vx);
		temp_y = (int)(y/ 6);

		for(int i = 1; i <= -temp_vx; i++)
		{
			temp_x = (int)floor((x - i) / 6);
			if(!currentMap->fetch_wallmask(temp_x, temp_y))
			{
				*vx = -(i - 1);
				return;
			} // if it is wall
		} //for i
	} // vx negative

} // check_x_col()



void Physics::check_y_wall(float *vx, float *vy, float x, float y, int width,
	int height,	MapData* currentMap)
{
	int temp_vy = 0;
	int temp_x, temp_y;
	
	if(*vy == 0)
		return;
	
	if(*vy > 0) // moving down
	{
		temp_vy = (int)ceil(*vy);
		temp_x = (int)floor((x + *vx) / 6);

		for(int i = 1; i <= temp_vy; i++)
		{
			temp_y = (int)floor((y + i) / 6);
			if(!currentMap->fetch_wallmask(temp_x, temp_y))
			{
				*vy = i - 1;
				return;
			} // if is wall
		} // for i
	} // vy positive


	if(*vy < 0) // moving up
	{
		temp_vy = (int)floor(*vy);
		temp_x = (int)floor((x + *vx) / 6);


		for(int i = 1; i <= -temp_vy; i++)
		{
			temp_y = (int)floor((y - i) / 6);
			if(!currentMap->fetch_wallmask(temp_x, temp_y))
			{
				*vy = -(i - 1);
				//*vy = 0;
				return;
			} // if is wall
		} // for i
	} // vy positive

} // check_y_col()




void Physics::gravity(float *vy)
{
	*vy += grav_a * TIMESTEP;
} // gravity()




void Physics::check_wall_col(float *vx, float *vy, float x, float y, int width,
	int height, MapData* currentMap)
{
	//simulate stepping into wall mask and correct velocity

	check_x_wall(vx, vy, x, y, width, height, currentMap);
	check_y_wall(vx, vy, x, y, width, height, currentMap);

} // check_collision()
