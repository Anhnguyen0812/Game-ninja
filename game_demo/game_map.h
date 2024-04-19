
#ifndef GAME_MAP_H	
#define GAME_MAP_H

#include "CommonFunction.h"
#include "BaseObject.h"
#define MAX_TILE 116

class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap(const char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
	void SetMap(Map& map_data) { game_map_ = map_data; }
	void render_gate(const Map& map_data, SDL_Renderer* screen);
	Map getMap()const { return game_map_; }
	void Load(const char* name, SDL_Renderer* screen) { gate.LoadImg(name, screen); }

private:
	Map game_map_;
	TileMat tile_mat[MAX_TILE];
	BaseObject gate;

};

#endif