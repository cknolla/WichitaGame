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
	SAFE_DELETE(currentMap);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void WichitaGame::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	// initialize textures (remember to add them to resetAll() and releaseAll())
	if(!changerTexture.initialize(graphics, "pictures/Village01.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));

	if(!blankTexture.initialize(graphics, "pictures/blank.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing blank texture"));

	if(!chestTexture.initialize(graphics, "pictures/chest00.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing chest texture"));


	TextureManager changer2Texture;


	if(!changer2Texture.initialize(graphics, "pictures/bomb.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing changer texture"));
	

	// character texture
	if (!characterTexture.initialize(graphics,TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));

	if (!redCharTexture.initialize(graphics,RED_TEST_CHAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing red character texture"));

	// initialize player character
	if (!player.initialize(this,34,34,2,&characterTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));

	// set player starting conditions
	player.setFrames(0,1);
	player.setFrameDelay(0.1f);
	player.setActive(true);
	player.setEdge(characterNS::COLLISION_BOX);

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
	loadMap(GRAVEYARD2, 10.0*TILE_WIDTH, 10.0*TILE_HEIGHT);

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
	static bool isBlue = true;

	sprintf_s(debugLineBuf, "Debug Text");

	// move right if left is not pressed or move right if left is not pressed
	if(input->isKeyDown(MOVE_RIGHT_KEY) && !input->isKeyDown(MOVE_LEFT_KEY)) {
		player.moveRight(frameTime);
	} else if(input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		player.moveLeft(frameTime);
	}

	// move up if down is not pressed or move down if up is not pressed
	if( input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY)) {
		player.moveUp(frameTime);
	} else if( input->isKeyDown(MOVE_DOWN_KEY) && !input->isKeyDown(MOVE_UP_KEY)) {
		player.moveDown(frameTime);
	}
	// set velocity to 0 in x or y direction if neither key is pressed
	if(!input->isKeyDown(MOVE_UP_KEY) && !input->isKeyDown(MOVE_DOWN_KEY)) {
		player.stopY();
	}
	if(!input->isKeyDown(MOVE_LEFT_KEY) && !input->isKeyDown(MOVE_RIGHT_KEY)) {
		player.stopX();
	}


	if(input->wasKeyPressed('C')) {
		if(isBlue) {
			if (!player.initialize(this,34,34,2,&redCharTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));
			isBlue = false;
		} else {
			if (!player.initialize(this,34,34,2,&characterTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character"));
			isBlue = true;
		}
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
		player.setCurrentFrame(player.getEndFrame());
		player.setVelocity(VECTOR2(0.0f, 0.0f));
		player.setLoop(false);
	} else {
		player.setLoop(true);
	}
	
	// Update the map BEFORE the character since it manipulates the player's position
	currentMap->update(player, frameTime);
	player.update(frameTime);

	sprintf_s(messageBuffer, "X: %.3f, Y: %.3f", player.getX(), player.getY());

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
	Tile* curTile = currentMap->getFirstTile();
	ZoneChanger* curChanger = currentMap->getFirstChanger();
	Chest* curChest = currentMap->getFirstChest();
	Door* curDoor = currentMap->getFirstDoor();

	if(!noclip) {
		// check tile collision only - no objects
		while(curTile) {
			solidObjectCollision(player, *curTile);
			curTile = curTile->getNextTile();
		}
		//currentMap->collisions(player);
		// check collision with map  objects
		while(curChanger) {
			if(player.collidesWith((*curChanger), collisionVector)) {
				// loadMap destroys the currentMap, so be cautious of the fact that anything referenced from currentMap won't exist when you get back from it
				loadMap(curChanger->getDestination(), curChanger->getDestinationStartX(), curChanger->getDestinationStartY());
				break; // after the new map is loaded, this zonechanger list doesn't exist anymore. Break to avoid access violations
			}
			curChanger = curChanger->getNextChanger();
		}
		while(curChest) {
			solidObjectCollision(player, *curChest);
			curChest = curChest->getNextChest();
		}
		while(curDoor) {
			if(player.collidesWith((*curDoor), collisionVector)) {
				curDoor->setCurrentFrame(1);
			} else {
				curDoor->setCurrentFrame(0);
			}
			curDoor = curDoor->getNextDoor();
		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void WichitaGame::render()
{
	Tile* curTile = currentMap->getFirstTile();
	ZoneChanger* curChanger = currentMap->getFirstChanger();
	NPC* curNPC = currentMap->getFirstNPC();
	Chest* curChest = currentMap->getFirstChest();
    graphics->spriteBegin();                // begin drawing sprites

	// Draw from bottom to top

	// draw bottom map layer
	while(curTile) {
		// only draw the tile if it's on screen
		if(onScreen(curTile)) {
				curTile->draw();
		}
		curTile = curTile->getNextTile();
	}
	
	// draw map layer 2
	curTile = currentMap->getLayer2FirstTile();
		while(curTile) {
		// only draw the tile if it's on screen
		if(onScreen(curTile)) {
				curTile->draw();
		}
		curTile = curTile->getNextTile();
	}

	while(curChanger) {
		if(onScreen(curChanger))
			curChanger->draw();
		curChanger = curChanger->getNextChanger();
	}
	while(curNPC) {
		if(onScreen(curNPC))
			curNPC->draw();
		curNPC = curNPC->getNextNPC();
	}
	while(curChest) {
		if(onScreen(curChest))
			curChest->draw();
		curChest = curChest->getNextChest();
	}

	player.draw();

	// draw map layer 3
	curTile = currentMap->getLayer3FirstTile();
		while(curTile) {
		// only draw the tile if it's on screen
		if(onScreen(curTile)) {
				curTile->draw();
		}
		curTile = curTile->getNextTile();
	}

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
// Unload current map, load a new map
//=============================================================================

bool WichitaGame::loadMap(MAP_LIST newMap, float startX, float startY)
{
	char errorStr[200];
	sprintf_s(errorStr, "Error initializing map");

	startY = startY - 2.0f; // character collides with things horizontal upon loading, so offset him a bit to avoid that
	
	// delete current map and its objects from memory if it exists
	SAFE_DELETE(currentMap);
	currentMap = new Map;
	////////////// GRAVEYARD ////////////////////////////////////////////////////////////////////
	// strcmp returns 0 if they match, so this statement is "if graveyard"
	if(newMap == GRAVEYARD) {
		if(!currentMap->initialize(this, GRAVEYARD_SET, GRAVEYARD_SET_SIZE, mapNS::GRAVEYARD_MAP_KEY))
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		else { // initialize map objects
			ZoneChanger* topChanger = currentMap->addChanger();
			ZoneChanger* bottomChanger = currentMap->addChanger();
			NPC* redGuy = currentMap->addNPC();

			if(!topChanger->initialize(TEST_MAP,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing graveyard changer 1"));

			if(!bottomChanger->initialize(TEST_MAP2,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing graveyard changer 2"));

			if(!redGuy->initialize(this,34,34,2,&redCharTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing redGuy"));

			topChanger->setStartingPos(8,0); // top center of graveyard
			topChanger->setDestinationStartingPos(3,5);
			bottomChanger->setStartingPos(8,16); // bottom center of graveyard
			bottomChanger->setDestinationStartingPos(6,6);
			redGuy->setStartingPos(8,7);
			redGuy->setMoseyEndingPos(10,10);
			redGuy->setCurrentFrame(0);

		}
	} 
	//////////////////// GRAVEYARD 2.0 ////////////////////////////////////////////////////////
	else if(newMap == GRAVEYARD2) {
		if(!currentMap->initialize(this, GRAVEYARD2_SET, GRAVEYARD2_SET_SIZE, mapNS::GRAVEYARD2_MAP_KEY))
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		else { // initialize map objects
			ZoneChanger* pitChanger = currentMap->addChanger();
			ZoneChanger* bottomChanger = currentMap->addChanger();
			NPC* redGuy = currentMap->addNPC();
			Chest* treasure = currentMap->addChest();
			Door* door = currentMap->addDoor();

			if(!pitChanger->initialize(GRAVEYARD,this,0, 0, 0, &blankTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pitChanger"));

			if(!bottomChanger->initialize(TEST_MAP2,this,0, 0, 0, &blankTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing graveyard changer 2"));

			if(!redGuy->initialize(this,34,34,2,&redCharTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing redGuy"));

			if(!treasure->initialize(this, 32, 32, 2, &chestTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing treasure"));

			if(!door->initialize(this,32,32,2, &doorTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing door"));

			pitChanger->setStartingPos(8,2);
			pitChanger->setDestinationStartingPos(10,10);

			bottomChanger->setStartingPos(8,16); // bottom center of graveyard
			bottomChanger->setDestinationStartingPos(6,6);

			redGuy->setStartingPos(4,8);
			redGuy->setMoseyEndingPos(12,8);
			redGuy->setCurrentFrame(2);

			treasure->setStartingPos(4,3);
		//	treasure->setDegrees(270.0);

			door->setStartingPos(5,6);
		

		}
	} 
	//////////////////// TESTMAP /////////////////////////////////////////////////////////////
	else if(newMap == TEST_MAP) {
		if(!currentMap->initialize(this, TEST_TILE_SET, TEST_SET_SIZE, mapNS::TEST_TILE_MAP_KEY))
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		else {
			ZoneChanger* graveyardChanger = currentMap->addChanger();
			if(!graveyardChanger->initialize(GRAVEYARD2,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing testMap changer"));

			graveyardChanger->setStartingPos(18,10);
			graveyardChanger->setDestinationStartingPos(8,3);
		}
	} 
	//////////////////// TESTMAP2 /////////////////////////////////////////////////////////////
	else if(TEST_MAP2) {
		if(!currentMap->initialize(this, TEST_TILE_SET, TEST_SET_SIZE, mapNS::TEST_TILE_MAP_KEY2)) 
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		else {
			ZoneChanger* graveyardChanger = currentMap->addChanger();
			if(!graveyardChanger->initialize(GRAVEYARD2,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing testMap2 changer"));

			graveyardChanger->setStartingPos(10,13);
			graveyardChanger->setDestinationStartingPos(8,13);
		}
	} 
	// ERROR
	else {
		sprintf_s(errorStr, "Map not found!");
		SAFE_DELETE(currentMap);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		return false;
	}
	currentMap->setStartX(startX);
	currentMap->setStartY(startY);
	currentMap->reset();
	player.setX(GAME_WIDTH/2); // move the player where he belongs on the new map
	player.setY(GAME_HEIGHT/2);
	return true;
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
	characterTexture.onLostDevice();
	redCharTexture.onLostDevice();
	changerTexture.onLostDevice();
	blankTexture.onLostDevice();
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
	currentMap->onResetDevice();
	characterTexture.onResetDevice();
	redCharTexture.onResetDevice();
	changerTexture.onResetDevice();
	blankTexture.onResetDevice();
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

inline bool WichitaGame::onScreen(Image* object)
{
	if(object->getX() > -TILE_WIDTH && object->getX() < GAME_WIDTH && object->getY() > -TILE_HEIGHT && object->getY() < GAME_HEIGHT)
		return true;
	else
		return false;
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

void WichitaGame::solidObjectCollision(Entity &object1, Entity &object2)
{
	VECTOR2 collisionVector;
	float tempX;
	float tempY;
	bool Xoffender = false;
	bool Yoffender = false;

	if(object1.collidesWith(object2, collisionVector)) {
		// save the destination location
		tempX = object1.getX();
		tempY = object1.getY();
		// place the character back on the X axis
		object1.setX(object1.getPrevX());
		if(object1.collidesWith(object2, collisionVector)) {
			// if there is still collision after placing him back where he came from on the X axis, then Y axis is offending
			Yoffender = true;
		}
		object1.setX(tempX); // put him back to new position to check Y
		object1.setY(object1.getPrevY()); // return him to previous Y to see if X is offending
		if(object1.collidesWith(object2, collisionVector)) {
			// if there is still collision after placing him back where he came from on the Y axis, then X axis is offending
			Xoffender = true;
		}
		object1.setY(tempY); // put him in new position
		if(Xoffender && !Yoffender) {
			object1.setX(object1.getPrevX()); // allow Y movement since Y didn't cause collision
		} else if(Yoffender && !Xoffender) {
			object1.setY(object1.getPrevY()); // allow X movement since X didn't cause collision
		} else if(Xoffender && Yoffender) {
			object1.setX(object1.getPrevX());
			object1.setY(object1.getPrevY()); // don't allow movement in either direction (corner)
		}	
	}
}
