#include "mapdata.h"



MapEntity::MapEntity(): entityType(0), x(0), y(0), w(0), h(0)
{
} // MapEntity()



MapData::MapData(const char* filename): is_loaded(0), width(0), height(0),
	wallmask(NULL), numEntities(0)
{
	std::string raw_level;
	png_structp png_ptr;
	png_infop info_ptr;
	is_loaded = 0;

	if(!read_png(filename, &png_ptr, &info_ptr)
		&& !load_level(&raw_level, &png_ptr, &info_ptr)
		&& !extract_data(&raw_level))
	{
		is_loaded = 1;
		printf("Map load successful!\n");
		map_name.assign(filename);
	} // if everything is ok
	else
		printf("Map failed to load!\n");


	// debugging prints out entities to console
	printf("Entity Projection");
	for(int i = 0; i < numEntities; i++)
		printf("%d: %d,%d\n", entities[i].entityType, entities[i].x, entities[i].y);

	// debugging prints out wallmask to console
	printf("\nWallmask Projection: (will seg fault if map isn't loaded)\n");
	for(int i = 0; i < (int)height; i++)
	{
		printf("\n");
		for(int j = 0; j< (int)width; j++)
		{
			if(wallmask[j][i] == 1)
				printf("M");
			else
				printf(" ");
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
} // MapData()



MapData::~MapData()
{
	printf("mapdata destructor\n");
	if(wallmask)
	{
		for(size_t i = 0; i < width; i++)
			if(wallmask[i])
				delete wallmask[i];

		delete [] wallmask;
	}
} // ~MapData()



int MapData::read_png(const char* filename, png_structp *png_ptr,
	png_infop *info_ptr)
{
	unsigned char sig[8];
	FILE *fp;

	if(!(fp = fopen(filename, "rb")))
	{
		fprintf(stderr, "[Map Error] Could not find and open file.\n");
		return 1;
	} // if can't open file

	fread(sig, 1, 8, fp);
	if(!png_check_sig(sig, 8)) 				// check if PNG
	{
		fprintf(stderr, "[Map Error] Not a PNG file.\n");
		return 1;
	} // if not png

	// Allocate memory for PNG pointers
	*png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	*info_ptr = png_create_info_struct(*png_ptr);

	if(!*png_ptr)
	{
		fprintf(stderr, "[Map Error] Could not allocate memory.\n");
		return 1;
	} // if out of memory
	
	if(!*info_ptr)
	{
		png_destroy_read_struct(png_ptr, NULL, NULL);
		fprintf(stderr, "[Map Error] Could not allocate memory.\n");
		return 1;
	} // if out of memory

	if (setjmp(png_jmpbuf(*png_ptr)))
	{
		png_destroy_read_struct(png_ptr, info_ptr, NULL);
    fprintf(stderr, "[Map Error] PNG error.\n");
		return 1;
	} // general PNG error

	png_init_io(*png_ptr, fp);
	png_set_sig_bytes(*png_ptr, 8);
	png_read_png(*png_ptr, *info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	return 0;
} // read_png()



int MapData::load_level(std::string *raw_level, png_structp *png_ptr,
	png_infop *info_ptr)
{
	const char GG2_TEXT_CHUNK_KEYWORD[] = "Gang Garrison 2 Level Data";
	png_textp text_ptr;
	int num_text;

	png_get_text(*png_ptr, *info_ptr, &text_ptr, &num_text); // extract text info

	for(int i = 0; i < num_text; i++)
		if(strcmp(GG2_TEXT_CHUNK_KEYWORD, text_ptr[i].key) == 0)
		{
			raw_level->assign(text_ptr[i].text);
			return 0;
		} // if GG2 Data chunk found in text

	fprintf(stderr, "[Map Error] GG2 Level Data not found.\n");
	return 1; // GG2 Data chunk not found!
} //load_leveldata()



//decodeWalkmaskToPng
int MapData::parse_wallmask(std::string *walkmaskSection)
{
	printf("%s\n", walkmaskSection->c_str());// debug: prints wallmask
	const std::string DIVIDER = "\x0a";

	std::string line;
	int scanpos = 0, lineEnd;
	size_t w_count = 0, h_count = 0;

	lineEnd = walkmaskSection->find(DIVIDER, scanpos); // Find DIVIDER
	if((unsigned int)lineEnd == std::string::npos)
	{
		fprintf(stderr, "[Map Error] Parsing DIVIDER not found.\n");
		return 1;
	} // if DIVIDER not found

	line = walkmaskSection->substr(scanpos, lineEnd); // get width
	width = atoi(line.c_str());
	scanpos = ++lineEnd;

	lineEnd = walkmaskSection->find(DIVIDER, scanpos); // Find DIVIDER
	if((unsigned int)lineEnd == std::string::npos)
	{
		fprintf(stderr, "[Map Error] Parsing DIVIDER not found.\n");
		return 1;
	} // if DIVIDER not found

	line = walkmaskSection->substr(scanpos, lineEnd - scanpos); // get height
	height = atoi(line.c_str());
	scanpos = ++lineEnd;

	if(width > 1000000 || height > 1000000)
	{
		fprintf(stderr, "[Map Error] A map dimension exeeds 1,000,000 pixels.\n");
		return 1;
	} // if map too big

	// Allocate for wallmask data
	wallmask = new unsigned int* [width];

	for(size_t i = 0; i < width; i++)
		wallmask[i] = new unsigned int [height];

	// Begin parse
	for(size_t imageIndex = 0;
		(imageIndex < width * height) &&
		((unsigned int)scanpos < walkmaskSection->length());
		imageIndex += 6, scanpos++)
	{
		unsigned int currentData = walkmaskSection->at(scanpos) - 32;

		for(int bitpos = 0; bitpos < 6; bitpos++)
		{
			if(w_count == width)
			{
				w_count = 0;
				h_count++;
			} // if image width reached go to another row

			if(currentData & (1 << (5 - bitpos)))
				wallmask[w_count][h_count] = 0;
			else
				wallmask[w_count][h_count] = 1;

			w_count++;
		} // for bitpos // imaggeIndex + bitpos
	} // for imageIndex

	return 0;
} // decode



//extract_PNG_leveldata
int MapData::extract_data(std::string *raw_level)
{
	const std::string ENTITY_TAG = "{ENTITIES}";
	const std::string END_ENTITY_TAG = "{END ENTITIES}";
	const std::string WALKMASK_TAG = "{WALKMASK}";
	const std::string END_WALKMASK_TAG = "{END WALKMASK}";
	const std::string DIVIDER = "\x0a";

	size_t entityTagPos = raw_level->find(ENTITY_TAG);
	size_t endEntityTagPos = raw_level->find(END_ENTITY_TAG);
	size_t walkmaskTagPos = raw_level->find(WALKMASK_TAG);
	size_t endWalkmaskTagPos = raw_level->find(END_WALKMASK_TAG);

	if(entityTagPos == std::string::npos
		|| endEntityTagPos == std::string::npos
		|| walkmaskTagPos == std::string::npos
		|| endWalkmaskTagPos == std::string::npos
		|| entityTagPos > endEntityTagPos
		|| walkmaskTagPos > endWalkmaskTagPos)
	{
		fprintf(stderr, "[Map Error] Map data keyword(s) invalid.\n");
		return 1;
	} // if tags not found or invalid

	// Extract walkmask section
	size_t startWalkmaskSection = walkmaskTagPos + WALKMASK_TAG.length() +
		DIVIDER.length();
	size_t walkmaskSectionLength = endWalkmaskTagPos - startWalkmaskSection -
		DIVIDER.length();
	std::string walkmaskSection = raw_level->substr(startWalkmaskSection,
		walkmaskSectionLength);

	// Extract entity tag section
	size_t startEntitySection = entityTagPos + ENTITY_TAG.length() +
		DIVIDER.length();
	size_t entitySectionLength = endEntityTagPos - startEntitySection -
		DIVIDER.length();
	std::string entitySection = raw_level->substr(startEntitySection,
		entitySectionLength);

	// Parse Data
	if(parse_wallmask(&walkmaskSection) || parse_entities(&entitySection))
		return 1;

	return 0;
} // extract_data()



int MapData::identify_entity(const char *entityName, int *w, int *h)
{
	if(!strcmp("medCabinet", entityName))
	{
		*w = 6;
		*h = 10;
		return MEDCABINET;
	} // medCabinet

	if(!strcmp("redspawn", entityName))
	{

		*w = 3;
		*h = 10;
		return REDSPAWN;
	} // medCabinet

	if(!strcmp("redteamgate", entityName))
	{
		*w = 1;
		*h = 10;
		return REDTEAMGATE;
	} // redteamgate

	if(!strcmp("spawnroom", entityName))
	{
		*w = 10;
		*h = 10;
		return SPAWNROOM;
	} // redteamgate

	if(!strcmp("redintel", entityName))
	{
		*w = 3;
		*h = 3;
		return REDINTEL;
	} // redteamgate

	if(!strcmp("blueintel", entityName))
	{
		*w = 3;
		*h = 3;
		return BLUEINTEL;
	} // redteamgate

	if(!strcmp("blueteamgate", entityName))
	{
		*w = 1;
		*h = 10;
		return BLUETEAMGATE;
	} // redteamgate

	if(!strcmp("bluespawn", entityName))
	{
		*w = 3;
		*h = 10;
		return BLUESPAWN;
	} // redteamgate

	fprintf(stderr, "[Map Warning] Unrecognized entity: %s\n", entityName);
	return -1;
} // entity_type



int MapData::parse_entities(std::string *entitySection)
{
	int type, x, y, w, h;
	char *name;
	MapEntity temp;

	//printf("Entity Section:\n%s\n", entitySection->c_str());
	//printf("%d, %d\n", width, height);
	// debug prints entity secion
	
	char *full = new char[entitySection->length() + 1];
	strcpy(full, entitySection->c_str());

	name = strtok(full, "\n\r");
	x = atoi(strtok(NULL, "\n\r"));
	y = atoi(strtok(NULL, "\n\r"));

	type = identify_entity(name, &w, &h);
	temp.entityType = type;
	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;
	entities.push_back(temp);
	numEntities++;

	while((name = strtok(NULL, "\n\r")))
	{
		x = atoi(strtok(NULL, "\n\r"));
		y = atoi(strtok(NULL, "\n\r"));

		type = identify_entity(name, &w, &h);
		temp.entityType = type;
		temp.x = x;
		temp.y = y;
		temp.w = w;
		temp.h = h;
		entities.push_back(temp);
		numEntities++;
	} // while not EOF

	delete full;
	return 0;
} // parse_entities()







int MapData::fetch_is_loaded()
{
	return is_loaded;
} // is_it_loaded()



std::string MapData::fetch_map_name()
{
	return map_name;
} // fetch_map_name()



void MapData::fetch_map_dimensions(unsigned int *width, unsigned int *height)
{
	*width = this->width;
	*height = this->height;
} // fetch_map_dimensions()


/*
int MapData::fetch_num_entities()
{
	return numEntities;
} // fetch_num_entities()
*/


unsigned int MapData::fetch_wallmask(unsigned int x, unsigned int y)
{
	// map pixel is 6x6 in ingame pixels. Divide by 6 before calling function

	if(x >= 0 && x < width && y >= 0 && y < height)
		return wallmask[x][y]; // a 0 return value means there is a wall
	
	return 0;
} // fetch_wallmask()



unsigned int MapData::fetch_entities(int num, int *x, int *y, int *w,int *h)
{

	if(num >= 0 && num < numEntities)
	{
		*x = entities[num].x;
		*y = entities[num].y;
		*w = entities[num].w;
		*h = entities[num].h;
		return entities[num].entityType;
	}
	
	fprintf(stderr, "[Map Warning] Entity index out of bounds.\n");
	return -1;
} // fetch_entities()



void MapData::get_rand_spawn(int team, unsigned int seed, int *x, int *y)
{
	if(team == 0)
		return;


	int rightTeam, spawnPoint, count = 0;
	std::vector<MapEntity> rightEntities;
	MapEntity tempEnt;

	if(team == 1)
		rightTeam = REDSPAWN;

	if(team == 2)
		rightTeam = BLUESPAWN;


	for(int i = 0; i < numEntities; i++)
	{
		if(entities[i].entityType == rightTeam)
		{
			tempEnt = entities[i];
			rightEntities.push_back(tempEnt);
			count++;
		} // if right spawn for team

	}

printf("\nafter == seed: %u, and count: %d\n", seed, count); // debug
	srand(seed);
	spawnPoint = rand() % count;
	
	*x = rightEntities[spawnPoint].x;
	*y = rightEntities[spawnPoint].y;

} // get_rand_spawn()




