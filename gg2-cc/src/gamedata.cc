#include "gamedata.h"


Gamedata::Gamedata(Audio* audio_manager, Render* render_manager, Input* inputs,
	const char *filename):
	counter(0), currentMap(NULL), audio_manager(audio_manager),
	render_manager(render_manager), inputs(inputs)
{
	load_map(filename); 
	add_player(1, true); // debugging
	//add_player(1, false);
} // Gamedata()



Gamedata::~Gamedata()
{
	if(currentMap != NULL)
		delete currentMap;
	
} // ~Gamedata()



void Gamedata::load_map(const char* mapName)
{
	if(currentMap)
		delete currentMap;

	ALLEGRO_PATH *exe_path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(exe_path, "resources/maps"); // specific folder
	al_set_path_filename(exe_path, mapName);

	if(exe_path)
	{
		const char *proper_path = al_path_cstr(exe_path, ALLEGRO_NATIVE_PATH_SEP);
		currentMap = new MapData(proper_path);
		render_manager->load_map(mapName);
	} // if path valid
	
	al_destroy_path(exe_path);
} // load_map



void Gamedata::update()
{
	render_manager->clear_queue();
	std::list<Entity*>::iterator entIterate;

	for(entIterate = entities.begin(); entIterate != entities.end(); entIterate++)
		(*entIterate)->update(tick);

	tick++;
} // update()



void Gamedata::add_player(int character, bool isLocal)
{
	Entity *new_player;
	Input *tempInput = NULL;

	if(isLocal)
		tempInput = inputs;
	else
		tempInput = new Input; // dummy inputs


	switch(character)
	{
		case 0: // CLASS_SPECTATOR
			new_player = (Entity*) new Spectator(counter, audio_manager,
				render_manager, tempInput, currentMap, isLocal, this);
			break;
		case 1:
			new_player = (Entity*) new RedScout(counter, audio_manager,
				render_manager, tempInput, currentMap, isLocal, this); // inputs null for "dummy"
			break;	
		default:
			break;
	};

	entities.push_back(new_player);
	counter++;
} // add_player()



void Gamedata::start_round()
{
	std::list<Entity*>::iterator entIterate;

	for(entIterate = entities.begin(); entIterate != entities.end(); entIterate++)
		(*entIterate)->spawn(); // do not add players with spawn function here

} // start_round()
