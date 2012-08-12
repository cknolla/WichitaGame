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
#include "character.h"

namespace mapNS {
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;
	const int TILE_DRAW_WIDTH = 34;
	const int TILE_DRAW_HEIGHT = 34;
	const int MAX_MAP_WIDTH = 2048/TILE_WIDTH;
	const int MAX_MAP_HEIGHT = 1536/TILE_HEIGHT; // 768*2
	const float CAMERA_TRIGGER = 150.0f; // used to decide how far from the screen's edge before shifting the 'camera'
	const float CAMERA_MOVE_SPEED = characterNS::MOVE_SPEED*2;
//	const char TEST_TILE_MAP_IMAGE[] = "pictures/tileset34x34.png";
	const char TEST_TILE_MAP_IMAGE[] = "pictures/tileset01.png";
	const char TEST_TILE_MAP_KEY[] = "maps/testMap01.txt";
	const char TEST_TILE_MAP_KEY2[] = "maps/testMap02.txt";
	
	const RECT TILE_COLLISION_BOX = {-TILE_WIDTH/2, -TILE_HEIGHT/2, TILE_WIDTH/2, TILE_HEIGHT/2};
}

class Map {
private:
	TextureManager mapTexture;
	Entity tile[mapNS::MAX_MAP_HEIGHT][mapNS::MAX_MAP_WIDTH];
	bool initialized;
	int width;
	int height;
	// player starting position
	float startX;
	float startY;

public:
	Map();
	~Map();
	bool initialize(Game* gamePtr, const char* textureFile, const char* keyFile);
	void update(Character& player, float frameTime);

	// get map width in tiles
	int getWidth() { return width; }
	// get map height in tiles
	int getHeight() { return height; }
	// pull a single tile from the map
	Entity* getTile(int row, int col);
	// get player's starting positions
	float getStartX() { return startX; }
	float getStartY() { return startY; }

	// set player starting positions
	void setStartX(float x) { startX = x; }
	void setStartY(float y) { startY = y; }

	// handle texture if device lost
	void onLostDevice();
	void onResetDevice();
};

#endif
