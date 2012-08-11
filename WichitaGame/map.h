// Map class

#ifndef _MAP_H
#define _MAP_H
#define WIN32_LEAN_AND_MEAN

#include <istream>
#include <ostream>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "entity.h"

namespace mapNS {
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;
	const int MAX_MAP_WIDTH = 2048/TILE_WIDTH;
	const int MAX_MAP_HEIGHT = 1536/TILE_HEIGHT; // 768*2
	const char TEST_TILE_MAP_IMAGE[] = "pictures/tileset01.png";
	const char TEST_TILE_MAP_KEY[] = "maps/testMap01.txt";
	
	const RECT TILE_COLLISION_BOX = {-TILE_WIDTH/2, -TILE_HEIGHT/2, TILE_WIDTH/2, TILE_HEIGHT/2};
}

class Map {
private:
	TextureManager mapTexture;
	Entity tile[mapNS::MAX_MAP_HEIGHT][mapNS::MAX_MAP_WIDTH];
	bool initialized;
	int width;
	int height;

public:
	Map();
	~Map();
	bool initialize(Game* gamePtr, const char* textureFile, const char* keyFile);

	int getWidth() { return width; }
	int getHeight() { return height; }
	Entity* getTile(int row, int col);

	void onLostDevice();
	void onResetDevice();
};

#endif
