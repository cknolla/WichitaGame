// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h v1.0

#ifndef _WICHITAGAME_H           // prevent multiple definitions if this 
#define _WICHITAGAME_H           // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "character.h"

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
	Character testChar;
	TextureManager mapTexture;
	Image map;
	TextureManager tileMapTexture;
	Entity tileMap[MAP_HEIGHT/TILE_HEIGHT][MAP_WIDTH/TILE_WIDTH];
    TextDX  *dxFont;            // DirectX font
    std::string  message;
    float messageY;

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
};

#endif