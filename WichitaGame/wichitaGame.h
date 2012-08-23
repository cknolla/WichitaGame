// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h v1.0

#ifndef _WICHITAGAME_H           // prevent multiple definitions if this 
#define _WICHITAGAME_H           // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <iostream>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "character.h"
#include "map.h"
#include "tilesets.h"
#include "zoneChanger.h"
#include "npc.h"
#include "chest.h"
#include "door.h"
#include "bgfg.h"
#include <list>
#include <iterator>

//#include "itemspawn.h"
//forward declaration
class ItemSpawn;

using std::list;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"xinput.lib")

namespace wichitaGameNS
{
	const enum COLLISION_BOX_MASK {
		PLAYER_MASK =  1<<0,
		TILE_MASK =    1<<1,
		CHANGER_MASK = 1<<2,
		NPC_MASK =     1<<3,
		CHEST_MASK =   1<<4,
		DOOR_MASK =    1<<5,
	};
		
};

//=============================================================================
// This class is the core of the game
//=============================================================================
class WichitaGame : public Game
{
private:
    // game items
    TextureManager menuTexture; // textures
	TextureManager characterTexture;
	TextureManager redCharTexture;
	TextureManager changerTexture;
	TextureManager blankTexture;
	TextureManager chestTexture;
	TextureManager doorTexture;
	TextureManager foregroundTexture;

	Character player;

	Map* currentMap;
	Map testMap;
    TextDX  *dxFont;            // DirectX font
	TextDX *debugLine;
    std::string  message;
	TextureManager spawnTexture;
	//ItemSpawn* itemSpawn;
	list<ItemSpawn*> spawnList;
	int spawnCount;
    float messageY;
	char messageBuffer[1000];
	char debugLineBuf[1000];
	std::ofstream debugFile;
	bool noclip;
	bool tileNumbers;
	unsigned int collisionBoxMask;
	/*
	bool collisionBoxPlayer;
	bool collisionBoxTile;
	bool collisionBoxChanger;
	bool collisionBoxNPC;
	bool collisionBoxChest;
	bool collisionBoxDoor;
	*/
	VertexC vtx[4];                     // vertex data for collision boxes
    LP_VERTEXBUFFER vertexBuffer;       // buffer to hold collision box vertices

public:
    // Constructor
    WichitaGame();
    // Destructor
    virtual ~WichitaGame();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
	void createItemSpawn();
	bool destroyItemSpawn();
	//bool itemSpawnExists(){ if(itemSpawn != NULL) return true; return false; }
	bool itemSpawnExists();

	// check if object's position makes it visible on screen
	inline bool onScreen(Image* object);

	// fill screen with image (used for foreground/background). Image must be at least size of screen
	void fillScreen(Image* image);

	// Replace Game's consoleCommand to create custom commands
	void consoleCommand();

	// switch to new map, draw character
	void changeMap(Map &newMap);
	// unload previous map from memory, load a new one, and place character at startX, startY
	bool loadMap(MAP_LIST newMap, float startX = 0.0, float startY = 0.0);

	// draw a semi-transparent box representing the collision area for an entity
	void drawCollisionBox(Entity* object, COLOR_ARGB color);

	// object 1 is usually player. If player hits collidable object 2, resist his movement
	void solidObjectCollision(Entity &object1, Entity &object2);
};

#endif
