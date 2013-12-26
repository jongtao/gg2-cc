#ifndef MAPDATA_H
#define MAPDATA_H

#include <png.h> // remember compile with -lpng
#include <string>
#include <cstdint> // with -std=c++11
#include <cstdio>
#include <cstdlib>

class MapData
{
private:
	int is_loaded; // 1 = loaded
	std::string map_name;
	unsigned int width;
	unsigned int height;
	uint8_t **wallmask; //0 is empty; 1 is wall. [width][height]
	uint8_t **entities;

	int read_png(const char *filename, png_structp *png_ptr,
		png_infop *info_ptr);
	int load_level(std::string *raw_level, png_structp *png_ptr,
		png_infop *info_ptr);
	int extract_data(std::string *raw_level);
	int parse_wallmask(std::string *wallmaskSection);
	int parse_entities(std::string *entitySectionWithTags_return);

public:
	MapData(const char *filename);
	~MapData();

	int fetch_is_loaded();
	std::string fetch_map_name();
	void fetch_map_dimensions(unsigned int *width, unsigned int *height);
	uint8_t fetch_wallmask(unsigned int x, unsigned int y);
	uint8_t fetch_entities(unsigned int x, unsigned int y);
}; // MapData

#endif
