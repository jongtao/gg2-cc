#ifndef MAPDATA_H
#define MAPDATA_H

#include <png.h> // remember compile with -lpng
#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>



enum entities
{
	MEDCABINET,
	REDSPAWN,
	REDTEAMGATE,
	SPAWNROOM,
	REDINTEL,
	BLUEINTEL,
	BLUETEAMGATE,
	BLUESPAWN
}; // enum entities



class MapEntity
{
public:
	int entityType;
	int x, y, w, h;

	MapEntity();
}; // MapEntity



class MapData
{
private:
	int is_loaded; // 1 = loaded
	std::string map_name;
	unsigned int width;
	unsigned int height;
	unsigned int **wallmask; //0 is empty; 1 is wall. [width][height]
	std::vector<MapEntity> entities; // 6x6 per wallmask pixel
	int numEntities;

	int read_png(const char *filename, png_structp *png_ptr,
		png_infop *info_ptr);
	int load_level(std::string *raw_level, png_structp *png_ptr,
		png_infop *info_ptr);
	int extract_data(std::string *raw_level);

	int parse_wallmask(std::string *wallmaskSection);

	int identify_entity(const char *entityName, int *w, int *h);
	int parse_entities(std::string *entitySection);

public:
	MapData(const char *filename);
	~MapData();

	int fetch_is_loaded();
	std::string fetch_map_name();
	void fetch_map_dimensions(unsigned int *width, unsigned int *height);
	//int fetch_num_entities();

	unsigned int fetch_wallmask(unsigned int x, unsigned int y);
	unsigned int fetch_entities(int num, int *x, int *y, int *w, int *h);

	void get_rand_spawn(int team, unsigned int seed, int *x, int *y);

}; // MapData

#endif
