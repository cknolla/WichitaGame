// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.cpp v1.0
// This class is the core of the game

#include "wichitaGame.h"
#include "itemspawn.h"

//Comment out these lines if you do not want a console window defined


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
	//itemSpawn = NULL;
	
	spawnCount = 0;
}

//=============================================================================
// Destructor
//=============================================================================
WichitaGame::~WichitaGame()
{
	debugFile.close();
	destroyItemSpawn();
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

	// initialize map which will initialize a texture and lots of images inside it
	if(!testMap.initialize(this, TEST_TILE_SET, mapNS::TEST_TILE_MAP_KEY))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	if(!testMap2.initialize(this, TEST_TILE_SET, mapNS::TEST_TILE_MAP_KEY2))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	if(!graveyard.initialize(this, GRAVEYARD_SET, mapNS::GRAVEYARD_MAP_KEY))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map"));

	// Must assign currentMap to a map here
	currentMap = &graveyard;

	if(!changerTexture.initialize(graphics, "pictures/Village01.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));

	if(!graveyardChanger1.initialize(&testMap,this,0, 0, 0, &changerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer 1"));

	if(!graveyardChanger2.initialize(&testMap2,this,0, 0, 0, &changerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer 2"));

	if(!testMapChanger.initialize(&graveyard,this,0, 0, 0, &changerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing testMap changer"));
	if(!testMap2Changer.initialize(&graveyard,this,0, 0, 0, &changerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing testMap2 changer"));

	graveyardChanger1.setStartingPos(8,0); // top center of graveyard
	graveyardChanger2.setStartingPos(8,16); // bottom center of graveyard
	testMapChanger.setStartingPos(18,10);
	testMap2Changer.setStartingPos(10,13);

	TextureManager changer2Texture;
	ZoneChanger zoneChanger;

	if(!changer2Texture.initialize(graphics, "pictures/bomb.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));

	if(!zoneChanger.initialize(&testMap,this,0, 0, 0, &changer2Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer"));

	zoneChanger.setX(400.0f);
	zoneChanger.setY(300.0f);

	// add zone changers to current map's objects so that it moves along with it
	graveyard.addObject(graveyardChanger1);
	graveyard.addObject(graveyardChanger2);

	testMap.addObject(testMapChanger);

	testMap2.addObject(testMap2Changer);

	// character texture
	if (!characterTexture.initialize(graphics,TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	if (!testChar.initialize(this,34,34,2,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));

	testChar.setFrames(0,1);
	testChar.setFrameDelay(0.1f);
	testChar.setActive(true);
	testChar.setEdge(characterNS::COLLISION_BOX);

	//For spawn items
	if(!spawnTexture.initialize(graphics, "pictures/bomb.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));


	// initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 12, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	 if(debugLine->initialize(graphics, 16, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Debug Line"));

	messageY = GAME_HEIGHT-100.0f;

	// load the current map
	changeMap(*currentMap);

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

	if(input->wasKeyPressed('B')) {
		createItemSpawn();
	}
	if(input->wasKeyPressed('N')) {
		if(itemSpawnExists()){
			//printf("ItemSpawnExists");
			destroyItemSpawn();
		}
		//}else
		//	printf("ItemSpawnDoesNoTExist");
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
		if(currentMap == &graveyard) {
			if(testChar.collidesWith(graveyardChanger1, collisionVector))
			{
				changeMap(*graveyardChanger1.getDestination());
			}
			if(testChar.collidesWith(graveyardChanger2, collisionVector))
			{
				changeMap(*graveyardChanger2.getDestination());
			}
		}
		if(currentMap == &testMap) {
			if(testChar.collidesWith(testMapChanger, collisionVector))
				changeMap(*testMapChanger.getDestination());
		}
		if(currentMap == &testMap2) {
			if(testChar.collidesWith(testMap2Changer, collisionVector))
				changeMap(*testMap2Changer.getDestination());
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

	// Draw from bottom to top

	curTile = currentMap->getFirstTile();
	while(curTile) {
		// only draw the tile if it's on screen
		if(curTile->getX() > -TILE_WIDTH && curTile->getX() < GAME_WIDTH && curTile->getY() > -TILE_HEIGHT && curTile->getY() < GAME_HEIGHT) {
			curTile->draw();
		}
		curTile = curTile->getNextTile();
	}

	for(std::list<Entity*>::iterator curObject = currentMap->getObjects()->begin(); curObject != currentMap->getObjects()->end(); curObject++) {
		(*curObject)->draw();
	}

	testChar.draw();

	if(itemSpawnExists()){
	//	itemSpawn->draw();
		for(list<ItemSpawn*>::iterator i = spawnList.begin() ; i != spawnList.end() ; ++i ){
			(*i)->draw();
		}

	}

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
	currentMap = &newMap;
	currentMap->reset(); // reset the old map
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
	graveyard.onLostDevice();
	characterTexture.onLostDevice();
	changerTexture.onLostDevice();
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
	graveyard.onResetDevice();
	currentMap->onResetDevice();
	characterTexture.onResetDevice();
	changerTexture.onResetDevice();
    Game::resetAll();
    return;
}

void WichitaGame::createItemSpawn(){
//ItemSpawn createItemSpawn(){


	//Temporary Setup Just to make sure there are not memory leaks
	//if( itemSpawnExists() )
	//	destroyItemSpawn();

	//TextureManager* spawnTexture = new TextureManager();
	ItemSpawn* rItemSpawn = new ItemSpawn();
	float xPos = 0;
	float yPos = 0;

	int xTile = 0;
	int yTile = 0;

	//printf("Creating Item Spawn\n");	
	//Avoid divide by zero
	if( currentMap->getWidth() != 0 && currentMap->getHeight() != 0 ){
		
		xTile =  spawnCount % currentMap->getWidth();
		yTile = spawnCount / currentMap->getHeight();
	}
	//printf("Map Tile Offset X: %d , Y %d\n" , xTile , yTile );	
	currentMap->getXY(xPos , yPos , xTile , yTile );		
	//printf("Map Pos Offset X: %f , Y %f\n" , xPos , yPos );	
	rItemSpawn->setX( xPos );
	rItemSpawn->setY( yPos );	
	//printf("SpawnCount = %d" , spawnCount);
	//printf("Map Title Width: %d , Tile Height %d\n" , mapNS::TILE_WIDTH ,  mapNS::TILE_HEIGHT);	


	if(!rItemSpawn->initialize(&testMap,this,0, 0, 0, &spawnTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer"));

	++spawnCount;
	spawnList.push_back(rItemSpawn);
	//spawnList.insert(spawnList.begin() , rItemSpawn);

	//return rItemSpawn;
	return;
	

}

bool WichitaGame::destroyItemSpawn(){

	if(spawnList.empty() ){
		//printf("Empty");
		return false;
	}else{
		while(!spawnList.empty()){
			delete spawnList.front();
			spawnList.pop_front();
			--spawnCount;
		}


	}

	/*if(itemSpawn != NULL ){
		//printf("Deleting\n");
		delete itemSpawn;
		itemSpawn = NULL;
		//printf("PostDelete\n");
		--spawnCount;
		return true;
	}*/

	return true;
}

bool WichitaGame::itemSpawnExists(){
	return !spawnList.empty();
}
