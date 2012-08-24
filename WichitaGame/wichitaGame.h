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
#include "battle.h"
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

//=============================================================================
// This class is the core of the game
//=============================================================================
class WichitaGame : public Game
{
private:
    // game items
    TextureManager menuTexture; // textures
    Image   menu;               // menu image
	TextureManager characterTexture;
	TextureManager changerTexture;

	Character testChar;
	Map testMap;
	Map testMap2;
	Map graveyard;
	Map* currentMap;
	ZoneChanger graveyardChanger1;
	ZoneChanger graveyardChanger2;
	ZoneChanger testMapChanger;
	ZoneChanger testMap2Changer;
    TextDX  *dxFont;            // DirectX font
	TextDX *debugLine;
    std::string  message;
	TextureManager spawnTexture;
	//Actual battle entity
	Battle battle; 
	//Battle background texture
	TextureManager battleBackgroundTexture;
	//ItemSpawn* itemSpawn;
	list<ItemSpawn*> spawnList;
	int spawnCount;
    float messageY;
	char messageBuffer[1000];
	char debugLineBuf[1000];
	std::ofstream debugFile;
	bool noclip;
	//Battle definition bool
	bool battleOn;

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

	void consoleCommand();

	void changeMap(Map &newMap);
	//Battle function initializer 
	void WichitaGame::battleStart(const char* file);
};
#endif
