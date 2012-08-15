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
	debugLine = new TextDX();
    messageY = 0;
	debugFile.open("debugFile.txt");
	if(!debugFile.is_open())
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening debug file"));
	currentMap = NULL;
	noclip = false;
}

//=============================================================================
// Destructor
//=============================================================================
WichitaGame::~WichitaGame()
{
	debugFile.close();
    releaseAll();           // call onLostDevice() for every graphics item
    SAFE_DELETE(dxFont);
	SAFE_DELETE(debugLine);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void WichitaGame::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	//=============
#ifdef CONSOLE
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
#endif
	//=========================


	printf("Test");

	// initialize map which will initialize a texture and lots of images inside it
	if(!testMap.initialize(this, TEST_TILE_SET, mapNS::TEST_TILE_MAP_KEY))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	if(!testMap2.initialize(this, TEST_TILE_SET, mapNS::TEST_TILE_MAP_KEY2))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	// call changeMap() to change maps. Must assign currentMap to a map here
	currentMap = &testMap;
	changeMap(testMap2);

	if(!changerTexture.initialize(graphics, "pictures/testSet003.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));

	if(!testChanger.initialize(&testMap,this,0, 0, 0, &changerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer"));

	testChanger.setX(300.0f);
	testChanger.setY(300.0f);

	// character texture
	if (!characterTexture.initialize(graphics,TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	if (!testChar.initialize(this,34,34,2,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));

	testChar.setFrames(0,1);
	testChar.setFrameDelay(0.1f);
	testChar.setActive(true);
	testChar.setEdge(characterNS::COLLISION_BOX);

	// initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 12, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	 if(debugLine->initialize(graphics, 16, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Debug Line"));

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
	if(!currentMap)
		throw(GameError(gameErrorNS::FATAL_ERROR, "currentMap is NULL! Assign it to a map in WichitaGame::initialize!"));
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
	sprintf_s(debugLineBuf, "Debug line - update me");

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

	if(input->wasKeyPressed('Z')) {
		changeMap(testMap);	
	}
	if(input->wasKeyPressed('X')) {
		changeMap(testMap2);
	}
	
	// if no movement keys are pressed, draw the ending frame for the direction he's currently facing and pause animation
	if( !input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY) && !input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		testChar.setCurrentFrame(testChar.getEndFrame());
		testChar.setVelocity(VECTOR2(0.0f, 0.0f));
		testChar.setLoop(false);
	} else {
		testChar.setLoop(true);
	}
	
	// Update the map BEFORE the character since it manipulates the player's position
	currentMap->update(testChar, frameTime);
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
	float tempX;
	float tempY;
	bool Xoffender = false;
	bool Yoffender = false;
	Tile* curTile;

	if(!noclip) {
		curTile = currentMap->getFirstTile(); 
		while(curTile) {
			if(testChar.collidesWith(*curTile, collisionVector)) {
				sprintf_s(debugLineBuf, "Collision!");
				// save the destination location
				tempX = testChar.getX();
				tempY = testChar.getY();
				// place the character back on the X axis
				testChar.setX(testChar.getPrevX());
				if(testChar.collidesWith(*curTile, collisionVector)) {
					// if there is still collision after placing him back where he came from on the X axis, then Y axis is offending
					Yoffender = true;
				}
				testChar.setX(tempX); // put him back to new position to check Y
				testChar.setY(testChar.getPrevY()); // return him to previous Y to see if X is offending
				if(testChar.collidesWith(*curTile, collisionVector)) {
					// if there is still collision after placing him back where he came from on the Y axis, then X axis is offending
					Xoffender = true;
				}
				testChar.setY(tempY); // put him in new position
				if(Xoffender && !Yoffender) {
					testChar.setX(testChar.getPrevX()); // allow Y movement since Y didn't cause collision
				} else if(Yoffender && !Xoffender) {
					testChar.setY(testChar.getPrevY()); // allow X movement since X didn't cause collision
				} else if(Xoffender && Yoffender) {
					testChar.setX(testChar.getPrevX());
					testChar.setY(testChar.getPrevY()); // don't allow movement in either direction (corner)
				}	
			}
			curTile = curTile->getNextTile();
		}
		if(testChar.collidesWith(testChanger, collisionVector))
		{
			changeMap(*testChanger.getDestination());
			sprintf_s(debugLineBuf, "Collision with ZoneChanger!");
		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void WichitaGame::render()
{
	Tile* curTile;
    graphics->spriteBegin();                // begin drawing sprites

 //   menu.draw();
//	map.draw();
	/*
	for(int row = 0; row < currentMap->getHeight(); row++) { 
		for(int col = 0; col < currentMap->getWidth(); col++) { 
			currentMap->getTile(row, col)->draw();
		}
	}
	*/
	curTile = currentMap->getFirstTile();
	while(curTile) {
		curTile->draw();
		curTile = curTile->getNextTile();
	}

	testChanger.draw();

	testChar.draw();
    dxFont->setFontColor(graphicsNS::WHITE);
	debugLine->setFontColor(graphicsNS::WHITE);
 //   dxFont->print(message,20,(int)messageY);
	dxFont->print(messageBuffer, 20,(int)messageY);
	debugLine->print(debugLineBuf, 20, (int)messageY+20);

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Console commands
//=============================================================================
void WichitaGame::consoleCommand()
{

    command = console->getCommand();    // get command from console
    if(command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
		console->print("noclip - toggle clipping");
        return;
    }

    if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if(fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }

	if(command == "noclip")
	{
		noclip = !noclip;               // toggle clipping
		if(noclip)
			console->print("Noclip on");
		else
			console->print("Noclip off");
	}
}

//=============================================================================
// Cleanly change maps
//=============================================================================
void WichitaGame::changeMap(Map &newMap)
{
	currentMap->reset(); // reset the old map
	currentMap = &newMap;
	testChar.setX(GAME_WIDTH/2); // move the player where he belongs on the new map
	testChar.setY(GAME_HEIGHT/2);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void WichitaGame::releaseAll()
{
    dxFont->onLostDevice();
	debugLine->onLostDevice();
//    menuTexture.onLostDevice();
	testMap.onLostDevice();
	testMap2.onLostDevice();
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
    dxFont->onResetDevice();
	debugLine->onResetDevice();
	testMap.onResetDevice();
	testMap2.onResetDevice();
	currentMap->onResetDevice();
	characterTexture.onResetDevice();
    Game::resetAll();
    return;
}
