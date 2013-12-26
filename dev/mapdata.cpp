#include "mapdata.h"



MapData::MapData(const char* filename)
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

	// debugging prints out wallmask to console
	printf("Wallmask Projection:\n");
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
	if(is_loaded)
	{
		for(size_t i = 0; i < width; i++)
			delete wallmask[i];

		delete [] wallmask;
	} // if map is loaded
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
	wallmask = new uint8_t* [width];

	for(size_t i = 0; i < width; i++)
		wallmask[i] = new uint8_t [height];

	// Begin parse
	for(size_t imageIndex = 0;
		(imageIndex < width * height) &&
		((unsigned int)scanpos < walkmaskSection->length());
		imageIndex += 6, scanpos++)
	{
		uint8_t currentData = walkmaskSection->at(scanpos) - 32;

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



int MapData::parse_entities(std::string *entitySectionWithTags_return)
{
	printf("Entity Section:\n%s\n", entitySectionWithTags_return->c_str());
	// debug prints entity secion
	// will be replaced with entity parser
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



uint8_t MapData::fetch_wallmask(unsigned int x, unsigned int y)
{
	if(x >= 0 && x < width && y >= 0 && y < height)
		return wallmask[x][y];
	
	fprintf(stderr, "[Map Error] Wallmask index out of bounds.\n");
	return -1;
} // fetch_wallmask()



uint8_t MapData::fetch_entities(unsigned int x, unsigned int y)
{
	if(x >= 0 && x < width && y >= 0 && y < height)
		return entities[x][y];
	
	fprintf(stderr, "[Map Error] Entitiy index out of bounds.\n");
	return -1;
} // fetch_entities()
