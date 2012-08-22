// Map class

#ifndef _MAP_H
#define _MAP_H
#define WIN32_LEAN_AND_MEAN
// stupid strcpy warning
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "tile.h"
#include "zoneChanger.h"
#include "character.h"
#include "npc.h"
#include "chest.h"
#include "door.h"

namespace mapNS {
	const int MAX_LAYERS = 3;
	const float CAMERA_TRIGGER = 10*TILE_WIDTH; // used to decide how far from the screen's edge before shifting the 'camera'
	const float CAMERA_MOVE_SPEED = characterNS::MOVE_SPEED;
	const char TEST_TILE_MAP_IMAGE[] = "pictures/tileset01.png";
	const char TEST_TILE_MAP_KEY[] = "maps/testMap01.txt";
	const char TEST_TILE_MAP_KEY2[] = "maps/testMap02.txt";
	const char GRAVEYARD_MAP_KEY[] = "maps/graveyard.map";
	const char GRAVEYARD2_MAP_KEY[] = "maps/graveyard2.0.map";
	
	const RECT TILE_COLLISION_BOX = {-TILE_WIDTH/2, -TILE_HEIGHT/2, TILE_WIDTH/2, TILE_HEIGHT/2};
}

class Map {
private:
	Tile* firstTile;
	Tile* layer2firstTile;
	Tile* layer3firstTile;
	// adding a new object list? It must be accounted for in these places:
	// set the new 'first' pointer to NULL in map's constructor
	// map's update()
	// game's collisions()
	// get/add functions in map.h and map.cpp
	// map's unload()
	// map's reset()
	// game's render()
	TextureManager* firstTexture;
	ZoneChanger* firstChanger;
	NPC* firstNPC;
	Chest* firstChest;
	Door* firstDoor;
	Image* background;
	Image* foreground;
	bool initialized;
	int width;
	int height;
	// player starting position
	float startX;
	float startY;

public:
	Map();
	virtual ~Map();
	bool initialize(Game* gamePtr, const char* tileSet[], int tileSetSize, const char* keyFile);
	// scroll map when necessary
	void update(Character& player, float frameTime);
	// process collisions with player
	// replaced by WichitaGame::solidObjectCollision()
//	void collisions(Character& player);

	// get map width in tiles
	int getWidth() { return width; }
	// get map height in tiles
	int getHeight() { return height; }
	// pull a single tile from the map
//	Entity* getTile(int row, int col);
	// get player's starting positions
	float getStartX() { return startX; }
	float getStartY() { return startY; }

	// set player starting positions
	void setStartX(float x) { startX = x; }
	void setStartY(float y) { startY = y; }
	// set player starting position based on tile grid position
	void setStartingPos(int tileX, int tileY);

	// 'attach' an entity to the map
//	void addObject(Entity &newObject) { mapObjects.push_back(&newObject); }

//	std::list<Entity*>* getObjects() { return &mapObjects; }
	
	// convert x and y tile position to absolute x and y position
	void getXY(float & x , float & y , int tileX  = 0 , int tileY = 0 );

	// Return first tile in the linked list
	Tile* getFirstTile() { return firstTile; }

	// Set first tile in the linked list
	void setFirstTile(Tile* nt) { firstTile = nt; }

	// Return first tile in the linked list
	Tile* getLayer2FirstTile() { return layer2firstTile; }

	// Set first tile in the linked list
	void setLayer2FirstTile(Tile* nt) { layer2firstTile = nt; }

	// Return first tile in the linked list
	Tile* getLayer3FirstTile() { return layer3firstTile; }

	// Set first tile in the linked list
	void setLayer3FirstTile(Tile* nt) { layer3firstTile = nt; }

	// create image and use passed texture
	void setBackground(Graphics* g, TextureManager texture);

	// create image and use passed texture
	void setForeground(Graphics* g, TextureManager texture);

	// Return first ZoneChanger in linked list
	ZoneChanger* getFirstChanger() { return firstChanger; }

	// add a ZoneChanger to the changer list
	ZoneChanger* addChanger();

	NPC* getFirstNPC() { return firstNPC; }

	// add an NPC to the linked list
	NPC* addNPC();

	// Return first Chest in linked list
	Chest* getFirstChest() { return firstChest; }

	// add a ZoneChest to the Chest list
	Chest* addChest();

	// Return first Door in linked list
	Door* getFirstDoor() { return firstDoor; }

	// add a ZoneDoor to the Door list
	Door* addDoor();

	// reset tiles to their starting location
	void reset();

	// free all linked list memory
	void unload();

	// handle texture if device lost
	void onLostDevice();
	void onResetDevice();
};

#endif
