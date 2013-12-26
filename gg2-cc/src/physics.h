#ifndef PHYSICS_H	
#define PHYSICS_H

#include <cmath>
#include "mapdata.h"
#include "constants.h"
#include <cstdio> // debug

#define GRAV_A 30
#define AIR_FR 2
#define GROUND_FR 6
#define FRICTION 6


class Physics
{
private:
	float grav_a;


	void check_x_wall(float *vx, float *vy, float x, float y, int width,
		int height,	MapData* currentMap);
	void check_y_wall(float *vx, float *vy, float x, float y, int width,
		int height,	MapData* currentMap);

public:
	Physics();

	int on_ground(float x, float y, int width, MapData* currentMap);
	void friction(float *vx, float *vy);
	void gravity(float *vy);
	void check_wall_col(float *vx, float *vy, float x, float y, int height,
		int width,MapData* currentMap);
}; // class Physics


#endif
