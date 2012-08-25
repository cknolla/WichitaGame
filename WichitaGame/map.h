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
#include "bgfg.h"

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
	const enum COLLISION_BOX_MASK {
		PLAYER_MASK =  1<<0,
		TILE_MASK =    1<<1,
		CHANGER_MASK = 1<<2,
		NPC_MASK =     1<<3,
		CHEST_MASK =   1<<4,
		DOOR_MASK =    1<<5,
	};
}

class Map {
private:
	Tile* firstTile;
	Tile* layer2firstTile;
	Tile* layer3firstTile;
	Text tileNum;
	// adding a new object list? It must be accounted for in these places:
	// set the new 'first' pointer to NULL in map's constructor
	// map's update()
	// game's collisions()
	// get/add functions in map.h and map.cpp
	// map's unload()
	// map's reset()
	// game's render()
	// console collision box mask and command
	// Map::collisionBoxes()
	TextureManager* firstTexture;
	ZoneChanger* firstChanger;
	NPC* firstNPC;
	Chest* firstChest;
	Door* firstDoor;
	Bgfg* background;
	Bgfg* foreground;
	bool initialized;
	int width;
	int height;
	// player starting position
	float startX;
	float startY;
	unsigned int collisionBoxMask;
	VertexC vtx[4];                     // vertex data for collision boxes
    LP_VERTEXBUFFER vertexBuffer;       // buffer to hold collision box vertices

public:
	Map();
	virtual ~Map();
	bool initialize(Game* gamePtr, const char* tileSet[], int tileSetSize, const char* keyFile);
	// scroll map when necessary
	void update(Character& player, float frameTime);
	// render map and objects
	void render(Character* player);
	// fill screen with background/foreground
	void fillScreen(Image* image);
	// check if collision boxes need to be drawn
	void collisionBoxes(Graphics* graphics, Character* player);
	// draw a semi-transparent box representing the collision area for an entity
	void drawCollisionBox(Graphics* graphics, Entity* object, COLOR_ARGB color);
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

	// Get pointer to tileNum TextDX
	Text* getTileNum() { return &tileNum; };

	// get background image
	Bgfg* getBackground() { return background; }

	// create image and use passed texture
	void setBackground(Graphics* g, TextureManager* texture);

	// get foreground image
	Bgfg* getForeground() { return foreground; }

	// create image and use passed texture
	void setForeground(Graphics* g, TextureManager* texture);

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

	// return current mask
	unsigned int getCollisionBoxMask() { return collisionBoxMask; }

	// set new collision box mask
	void setCollsionBoxMask(unsigned int mask) { collisionBoxMask = mask; }


	// reset tiles to their starting location
	void reset();

	// free all linked list memory
	void unload();

	// handle texture if device lost
	void onLostDevice();
	void onResetDevice();
};

#endif
