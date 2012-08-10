// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.cpp v1.0
// This class is the core of the game

#include "wichitaGame.h"

//=============================================================================
// Constructor
//=============================================================================
WichitaGame::WichitaGame()
{
    dxFont = new TextDX();  // DirectX font
    messageY = 0;
}

//=============================================================================
// Destructor
//=============================================================================
WichitaGame::~WichitaGame()
{
    releaseAll();           // call onLostDevice() for every graphics item
    SAFE_DELETE(dxFont);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void WichitaGame::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	// map texture
    if (!mapTexture.initialize(graphics,TEST_MAP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map texture"));

    // map image
    if (!map.initialize(graphics,0,0,0,&mapTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	// character texture
	if (!characterTexture.initialize(graphics,TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	if (!testChar.initialize(this,32,32,2,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));

	testChar.setX(200);
	testChar.setY(200);
	testChar.setFrames(0,1);
	testChar.setFrameDelay(0.1f);

	/* saved for reference only

	// menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	menu.setDegrees(300);
    menu.setScale(0.002861f);

    message = "\n\n\nUtilizes Object Oriented C++ and DirectX\n\n";
    message += "Sprites with Transparency\n\n";
    message += "Three types of Collision Detection:\n";
    message += "     - Circular (Distance)\n";
    message += "     - Oriented Box\n";
    message += "     - Rotated Box\n\n";
    message += "XACT Audio\n\n";
    message += "Sprite and DirectX Text\n\n";
    message += "Tile Based Graphics\n\n";
    message += "TCP/IP and UDP/IP Network Support\n\n";
    messageY = GAME_HEIGHT;
*/
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void WichitaGame::update()
{
	/* saved for reference only
    if(menu.getDegrees() > 0)
    {
        menu.setDegrees(menu.getDegrees() - frameTime * 120);
        menu.setScale(menu.getScale() + frameTime * 0.4f);
    }
    else if(messageY > -400)
    {
        menu.setDegrees(0);
        menu.setY(menu.getY() - frameTime * 50);
        messageY -= frameTime * 50;
    }
    else    // start over
    {
        menu.setDegrees(300);
        menu.setScale(0.002861f);
        menu.setY(0);
        messageY = GAME_HEIGHT;
    }
	*/

	if(input->isKeyDown('D')) {
		testChar.moveRight(frameTime);
	} else if( input->isKeyDown('W')) {
		testChar.moveUp(frameTime);
	} else if( input->isKeyDown('A')) {
		testChar.moveLeft(frameTime);
	} else if( input->isKeyDown('S')) {
		testChar.moveDown(frameTime);
	}
	testChar.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void WichitaGame::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void WichitaGame::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void WichitaGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites

 //   menu.draw();
	map.draw();
	testChar.draw();
 //   dxFont->setFontColor(graphicsNS::ORANGE);
 //   dxFont->print(message,20,(int)messageY);

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void WichitaGame::releaseAll()
{
//    dxFont->onLostDevice();
//    menuTexture.onLostDevice();
	characterTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void WichitaGame::resetAll()
{
//    menuTexture.onResetDevice();
//    dxFont->onResetDevice();
	characterTexture.onResetDevice();
    Game::resetAll();
    return;
}
