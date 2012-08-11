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
	debugFile.open("debugFile.txt");
	if(!debugFile.is_open())
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening debug file"));
}

//=============================================================================
// Destructor
//=============================================================================
WichitaGame::~WichitaGame()
{
	debugFile.close();
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
	const int tileMapKey[MAP_HEIGHT/TILE_HEIGHT][MAP_WIDTH/TILE_WIDTH] = {
		1,1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,2,2,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,2,2,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,2,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,1,0,0,0,0,0,3,3,3,2,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,0,3,3,3,2,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,1,0,0,0,0,0,3,3,3,3,3,3,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,3,3,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,1,
		1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,
		1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,2,1,1,1,1,
		1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,2,1,1,1,1,
		1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,2,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,2,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	// map texture
    if (!mapTexture.initialize(graphics,TEST_MAP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map texture"));

    // map image
    if (!map.initialize(graphics,0,0,0,&mapTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	// tile map texture
	if (!tileMapTexture.initialize(graphics,TEST_TILE_MAP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile map texture"));

	for(int row = 0; row < MAP_HEIGHT/TILE_HEIGHT; row++) { // rows
		for(int col = 0; col < MAP_WIDTH/TILE_WIDTH; col++) { // cols
			if (!tileMap[row][col].initialize(this,TILE_WIDTH,TILE_HEIGHT,0,&tileMapTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
			tileMap[row][col].setCurrentFrame(tileMapKey[row][col]);
			tileMap[row][col].setY((float)TILE_HEIGHT*row);
			tileMap[row][col].setX((float)TILE_WIDTH*col);
			if(tileMapKey[row][col] != 1 && tileMapKey[row][col] != 3) { // if not a water or mountain tile
				tileMap[row][col].setActive(false);
			}
			tileMap[row][col].setCollisionType(entityNS::BOX);
		//	tileMap[row][col].setCollisionRadius(15.0f);
		}
	}


	// character texture
	if (!characterTexture.initialize(graphics,TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	if (!testChar.initialize(this,34,34,2,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));

	testChar.setX(200);
	testChar.setY(200);
	testChar.setFrames(0,1);
	testChar.setFrameDelay(0.1f);
//	testChar.setCollisionType(entityNS::BOX);
	testChar.setCollisionRadius(31.0f);

	// initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 12, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	messageY = GAME_HEIGHT-100.0f;

	/* saved for reference only

	// menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	

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
	message = "DEBUG TEXT";
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

	// move right if left is not pressed or move right if left is not pressed
	if(input->isKeyDown(MOVE_RIGHT_KEY) && !input->isKeyDown(MOVE_LEFT_KEY)) {
		testChar.moveRight(frameTime);
	} else if(input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		testChar.moveLeft(frameTime);
	}

	// move up if down is not pressed or move down if up is not pressed
	if( input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY)) {
		testChar.moveUp(frameTime);
	} else if( input->isKeyDown(MOVE_DOWN_KEY) && !input->isKeyDown(MOVE_UP_KEY)) {
		testChar.moveDown(frameTime);
	}
	// set velocity to 0 in x or y direction if neither key is pressed
	if(!input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY)) {
		testChar.stopY();
	}
	if(!input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		testChar.stopX();
	}
	
	// if no movement keys are pressed, draw the ending frame for the direction he's currently facing and pause animation
	if( !input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY) && !input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		testChar.setCurrentFrame(testChar.getEndFrame());
		testChar.setVelocity(VECTOR2(0.0f, 0.0f));
		testChar.setLoop(false);
	} else {
		testChar.setLoop(true);
	}
	testChar.update(frameTime);
	sprintf_s(messageBuffer, "X: %.3f, Y: %.3f", testChar.getX(), testChar.getY());

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
{
	VECTOR2 collisionVector;
	
	for(int row = 0; row < MAP_HEIGHT/TILE_HEIGHT; row++) { 
		for(int col = 0; col < MAP_WIDTH/TILE_WIDTH; col++) { 
			if(testChar.collidesWith(tileMap[row][col], collisionVector)) {
				// normalize the vector
				Graphics::Vector2Normalize(&collisionVector);
				// place the character back where he came from with minor padding
				testChar.setX(testChar.getX()+collisionVector.x);
				testChar.setY(testChar.getY()+collisionVector.y);
			}

		}
	}
	
}

//=============================================================================
// Render game items
//=============================================================================
void WichitaGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites

 //   menu.draw();
//	map.draw();
	for(int i = 0; i < MAP_HEIGHT/TILE_HEIGHT; i++) { // rows
		for(int j = 0; j < MAP_WIDTH/TILE_WIDTH; j++) { // cols
			tileMap[i][j].draw();
		}
	}
	testChar.draw();
    dxFont->setFontColor(graphicsNS::WHITE);
 //   dxFont->print(message,20,(int)messageY);
	dxFont->print(messageBuffer, 20,GAME_HEIGHT-100);

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
