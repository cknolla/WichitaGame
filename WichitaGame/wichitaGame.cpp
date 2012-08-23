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
	tileNumbers = false;
	collisionBoxMask = 0;
	//itemSpawn = NULL;
	vertexBuffer = NULL;
	
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

	if(!doorTexture.initialize(graphics, "pictures/door.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing door texture"));

	if(!foregroundTexture.initialize(graphics, "pictures/graveyard2.0/fogforeground.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing door texture"));


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
		// check for changers LAST because if a new map is loaded, other objects are destroyed
		while(curChanger) {
			if(player.collidesWith((*curChanger), collisionVector)) {
				// loadMap destroys the currentMap, so be cautious of the fact that anything referenced from currentMap won't exist when you get back from it
				loadMap(curChanger->getDestination(), curChanger->getDestinationStartX(), curChanger->getDestinationStartY());
				break; // after the new map is loaded, this zonechanger list doesn't exist anymore. Break to avoid access violations
			}
			curChanger = curChanger->getNextChanger();
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
	Door* curDoor = currentMap->getFirstDoor();
	Bgfg* bgImage = currentMap->getBackground();
	Bgfg* fgImage = currentMap->getForeground();

	Text* tileNum = NULL;
	char numBuffer[20];
	int row, col;


    graphics->spriteBegin();                // begin drawing sprites
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Can't draw sprites outside this area ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw from bottom to top

	if(bgImage) {
		fillScreen(bgImage);
	}

	// draw bottom map layer
	while(curTile) {
		// only draw the tile if it's on screen
		if(onScreen(curTile))
				curTile->draw();
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
		if(onScreen(curChanger)) {
			curChanger->draw();
		}
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
	while(curDoor) {
		if(onScreen(curDoor))
			curDoor->draw();
		curDoor = curDoor->getNextDoor();
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

	if(fgImage) {
		fillScreen(fgImage);
	}

	// print x,y position on tiles if debug option is enabled
	if(tileNumbers) {
		curTile = currentMap->getFirstTile();
		for(row = 0; row < currentMap->getHeight(); row++) {
			for(col = 0; col < currentMap->getWidth(); col++) {
				if(curTile) {
					if(onScreen(curTile)) {
						// tiles are drawn across, then down, so the row/col variables will align with curTile
						sprintf_s(numBuffer, "%d,%d", col, row);                                              // 4 pixel offset on odd numbered tiles
						currentMap->getTileNum()->print(numBuffer, (int)curTile->getX(), (int)curTile->getY()+(4*(col%2)));
					}
					curTile = curTile->getNextTile();
				}
			}
		}
	}

    dxFont->setFontColor(graphicsNS::WHITE);
	debugLine->setFontColor(graphicsNS::WHITE);
 //   dxFont->print(message,20,(int)messageY);
	dxFont->print(messageBuffer, 20,(int)messageY);
	debugLine->print(debugLineBuf, 20, (int)messageY+20);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    graphics->spriteEnd();                  // end drawing sprites
	
	// draw collision boxes if enabled through console
	if(collisionBoxMask & wichitaGameNS::TILE_MASK) {
		curTile = currentMap->getFirstTile();
		while(curTile) {
			if(onScreen(curTile))
				drawCollisionBox(curTile, graphicsNS::LIME & graphicsNS::ALPHA50);
			curTile = curTile->getNextTile();
		}
	}
	if(collisionBoxMask & wichitaGameNS::CHANGER_MASK) {
		curChanger = currentMap->getFirstChanger();
		while(curChanger) {
			if(onScreen(curChanger))
				drawCollisionBox(curChanger, graphicsNS::YELLOW & graphicsNS::ALPHA50);
			curChanger = curChanger->getNextChanger();
		}
	}
	if(collisionBoxMask & wichitaGameNS::NPC_MASK) {
		curNPC = currentMap->getFirstNPC();
		while(curNPC) {
			if(onScreen(curNPC))
				drawCollisionBox(curNPC, graphicsNS::GRAY & graphicsNS::ALPHA50);
			curNPC = curNPC->getNextNPC();
		}
	}
	if(collisionBoxMask & wichitaGameNS::CHEST_MASK) {
		curChest = currentMap->getFirstChest();
		while(curChest) {
			if(onScreen(curChest))
				drawCollisionBox(curChest, graphicsNS::CYAN & graphicsNS::ALPHA50);
			curChest = curChest->getNextChest();
		}
	}
	if(collisionBoxMask & wichitaGameNS::DOOR_MASK) {
		curDoor = currentMap->getFirstDoor();
		while(curDoor) {
			if(onScreen(curDoor))
				drawCollisionBox(curDoor, graphicsNS::PURPLE & graphicsNS::ALPHA50);
			curDoor = curDoor->getNextDoor();
		}
	}
	if(collisionBoxMask & wichitaGameNS::PLAYER_MASK)
		drawCollisionBox(&player, graphicsNS::RED & graphicsNS::ALPHA50);
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
		console->print("noclip (nc) - toggle clipping");
		console->print("tileNumbers (tn) - toggle display of x,y tile position");
		console->print("collisionBoxes (cb) - toggle display of non-tile collision boxes");
		console->print("To enable/disable individual collision boxes, use:");
		console->print("cb tile, cb player, cb changer, cb npc, cb chest, cb door");
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

	if(command == "nc" || command == "noclip")
	{
		noclip = !noclip;               // toggle clipping
		if(noclip)
			console->print("Noclip on");
		else
			console->print("Noclip off");
	}

	if(command == "tn" || command == "tileNumbers")
	{
		tileNumbers = !tileNumbers;               // toggle display of x,y tile position
		if(tileNumbers)
			console->print("Tile numbers on");
		else
			console->print("Tile numbers off");
	}

	if(command == "cb" || command == "collisionBoxes")
	{
		// toggle display of object collision boxes (not tiles)
		collisionBoxMask ^= wichitaGameNS::PLAYER_MASK | wichitaGameNS::CHANGER_MASK | wichitaGameNS::CHEST_MASK | wichitaGameNS::NPC_MASK | wichitaGameNS::DOOR_MASK;
		console->print("Non-tile collision boxes swapped");
	}

	if(command == "cb tile")
	{
		collisionBoxMask ^= wichitaGameNS::TILE_MASK;
		if(collisionBoxMask & wichitaGameNS::TILE_MASK)
			console->print("Tile collision boxes on");
		else
			console->print("Tile collision boxes off");
	}
	if(command == "cb player")
	{
		collisionBoxMask ^= wichitaGameNS::PLAYER_MASK;
		if(collisionBoxMask & wichitaGameNS::PLAYER_MASK)
			console->print("Player collision boxes on");
		else
			console->print("Player collision boxes off");
	}
	if(command == "cb changer")
	{
		collisionBoxMask ^= wichitaGameNS::CHANGER_MASK;
		if(collisionBoxMask & wichitaGameNS::CHANGER_MASK)
			console->print("Changer collision boxes on");
		else
			console->print("Changer collision boxes off");
	}
	if(command == "cb npc")
	{
		collisionBoxMask ^= wichitaGameNS::NPC_MASK;
		if(collisionBoxMask & wichitaGameNS::NPC_MASK)
			console->print("NPC collision boxes on");
		else
			console->print("NPC collision boxes off");
	}
	if(command == "cb chest")
	{
		collisionBoxMask ^= wichitaGameNS::CHEST_MASK;
		if(collisionBoxMask & wichitaGameNS::CHEST_MASK)
			console->print("Chest collision boxes on");
		else
			console->print("Chest collision boxes off");
	}
	if(command == "cb door")
	{
		collisionBoxMask ^= wichitaGameNS::DOOR_MASK;
		if(collisionBoxMask & wichitaGameNS::DOOR_MASK)
			console->print("Door collision boxes on");
		else
			console->print("Door collision boxes off");
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

			currentMap->setForeground(graphics, &foregroundTexture);
			currentMap->getForeground()->setAutoVscroll(-100.0);

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

			currentMap->setBackground(graphics, &foregroundTexture);
			currentMap->getBackground()->setParallax(0.5);

			pitChanger->setStartingPos(8,2);
			pitChanger->setDestinationStartingPos(10,10);

			bottomChanger->setStartingPos(8,16); // bottom center of graveyard
			bottomChanger->setDestinationStartingPos(6,6);

			redGuy->setStartingPos(4,8);
			redGuy->setMoseyEndingPos(12,8);
			redGuy->setCurrentFrame(2);

			treasure->setStartingPos(4,3);
		//	treasure->setDegrees(270.0);

			door->setStartingPos(4,6);
		

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

			currentMap->setForeground(graphics, &foregroundTexture);
			currentMap->getForeground()->setAutoHscroll(70.0);
			currentMap->getForeground()->setAutoVscroll(70.0);

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

			currentMap->setForeground(graphics, &foregroundTexture);
			currentMap->getForeground()->setAutoHscroll(150.0);

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

void WichitaGame::drawCollisionBox(Entity* object, COLOR_ARGB color)
{
	// an inactive entity never collides
	if(!object->getActive())
		return;

	// release buffer in order to create the next one
	SAFE_RELEASE(vertexBuffer);
	// top left
    vtx[0].x = object->getX()+(object->getWidth()/2)+(float)object->getEdge().left;
    vtx[0].y = object->getY()+(object->getHeight()/2)+(float)object->getEdge().top;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = color;

    // top right
    vtx[1].x = object->getX()+(object->getWidth()/2)+(float)object->getEdge().right;
    vtx[1].y = object->getY()+(object->getHeight()/2)+(float)object->getEdge().top;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = color;

    // bottom right
    vtx[2].x = object->getX()+(object->getWidth()/2)+(float)object->getEdge().right;
    vtx[2].y = object->getY()+(object->getHeight()/2)+(float)object->getEdge().bottom;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = color;

    // bottom left
    vtx[3].x = object->getX()+(object->getWidth()/2)+(float)object->getEdge().left;
    vtx[3].y = object->getY()+(object->getHeight()/2)+(float)object->getEdge().bottom;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = color;

//	if(!vertexBuffer)
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);

	graphics->drawQuad(vertexBuffer);       // draw collision box
}

void WichitaGame::fillScreen(Image* image)
{
	// save current location
	float prevX = image->getX();
	float prevY = image->getY();
	bool r0c0 = false, r0c1 = false, r0c2 = false, r1c0 = false, r1c1 = true, r1c2 = false, r2c0 = false, r2c1 = false, r2c2 = false;
	// r1c1 is always drawn. Up to 3 others can be drawn depending on position
	// r0c0 r0c1 r0c2
	// r1c0 r1c1 r1c2
	// r2c0 r2c1 r2c2

	// if the image is offscreen to the right, shift it a full width left
	if(image->getX() > GAME_WIDTH)
		image->setX(image->getX() - image->getWidth());
	// if offscreen to the left, shift it right
	if(image->getX() + image->getWidth() < 0)
		image->setX(image->getX() + image->getWidth());
	// if offscreen below, shift up
	if(image->getY() > GAME_HEIGHT)
		image->setY(image->getY() - image->getHeight());
	// if offscreen above, shift down
	if(image->getY() + image->getHeight() < 0)
		image->setY(image->getY() + image->getHeight());
	// now we know it's onscreen, so we'll draw it a max of 4 times to ensure the full screen is covered
	image->draw(); // draw it at its current location
	if(image->getX() > 0) // if left edge is showing
		r1c0 = true;
	if(image->getX() + image->getWidth() < GAME_WIDTH) // if right edge is showing
		r1c2 = true;
	if(image->getY() > 0) // if top edge is showing
		r0c1 = true;
	if(image->getY() + image->getHeight() < GAME_HEIGHT) // if bottom edge showing
		r2c1 = true;
	if(image->getX() > 0 && image->getY() > 0) // top left corner showing
		r0c0 = true;
	if(image->getY() > 0 && image->getX() + image->getWidth() < GAME_WIDTH) // top right corner
		r0c2 = true;
	if(image->getX() > 0 && image->getY() + image->getHeight() < GAME_HEIGHT) // bottom left corner
		r2c0 = true;
	if(image->getX() + image->getWidth() < GAME_WIDTH && image->getY() + image->getHeight() < GAME_HEIGHT) // bottom right corner
		r2c2 = true;

	if(r1c0) {
		image->setX(image->getX() - image->getWidth());
		image->draw();
		image->setX(prevX);
	}
	if(r0c1) {
		image->setY(image->getY() - image->getHeight());
		image->draw();
		image->setY(prevY);
	}
	if(r1c2) {
		image->setX(image->getX() + image->getWidth());
		image->draw();
		image->setX(prevX);
	}
	if(r2c1) {
		image->setY(image->getY() + image->getHeight());
		image->draw();
		image->setY(prevY);
	}
	if(r0c0) {
		image->setX(image->getX() - image->getWidth());
		image->setY(image->getY() - image->getHeight());
		image->draw();
		image->setX(prevX);
		image->setY(prevY);
	}
	if(r0c2) {
		image->setX(image->getX() + image->getWidth());
		image->setY(image->getY() - image->getHeight());
		image->draw();
		image->setX(prevX);
		image->setY(prevY);
	}
	if(r2c0) {
		image->setX(image->getX() - image->getWidth());
		image->setY(image->getY() + image->getHeight());
		image->draw();
		image->setX(prevX);
		image->setY(prevY);
	}
	if(r2c2) {
		image->setX(image->getX() + image->getWidth());
		image->setY(image->getY() + image->getHeight());
		image->draw();
		image->setX(prevX);
		image->setY(prevY);
	}
}
