#pragma once
#ifndef SAVEGAME_H
#define SAVEGAME_H
#include "game_map.h"
#include "MainObject.h"
#include "ThreatsObject.h"
#include "BossObject.h"
#include "CommonFunction.h"

class Savegame
{
public:
	Savegame();
	~Savegame();

	bool open(const char* name);
	bool read(const std::string& name);
	void savemap(const Map& map_data, const Map& map_data2);
	void saveplayer(MainObject p_player);
	void savethreat(std::vector<ThreatsObject*> list_threats);
	void saveboss(const BossObject& boss);
	void save_level(const int& level);
	void get(Map & map_data, Map& map_data2, MainObject& p_player);
	
	void getplayer(MainObject& p_player);
	void get_threats(std::vector<ThreatsObject*>& list_threats, SDL_Renderer* g_screen);
	void get_level(int& level);
	void close(){fi.close(),fo.close();}

private:
	std::ofstream fo;
	std::ifstream fi;

	int level_;
};

#endif // !SAVEGAME_H