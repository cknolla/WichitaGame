// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.cpp v1.0
// This class is the core of the game

#include "wichitaGame.h"

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
	currentBattle = NULL;
	noclip = false;
	
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
	battleEnd();
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

	if(!dialogBoxTexture.initialize(graphics, "pictures/dialogPlaceholder.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dialog box texture"));

	if(!gameMenu.initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game menu"));


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

	if (!dialogBox.initialize(graphics,0,0,0,&dialogBoxTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dialog box"));

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
	sprintf_s(debugLineBuf, "Debug Text");
	if (!currentBattle && !gameMenu.getActive()) {
		
		if(input->wasKeyPressed(gameConfig->getMenuKey())) {
			gameMenu.setActive(true);
			return; // menu has been activated, so escape this function to avoid input conflict
		}
		
		if(input->wasKeyPressed('I')) {
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

		// ACTION KEY LIST - Listed from highest priority to lowest. Each action clears the key, so only one is performed
		if(input->wasKeyPressed(gameConfig->getActionKey())) {
			Chest* curChest = currentMap->getFirstChest();
			while(curChest) {
				if(curChest->update(player))
					input->clearKeyPress(gameConfig->getActionKey()); // clear the key only if a chest opens
				curChest = curChest->getNextChest();
			}
		}
		
		//Press B to start a battle
		if(input->wasKeyPressed('B')) {
			battleStart("pictures/battle/battle_background.jpg");
			return; // battle has begun. escape function to avoid input conflict
		}
		// Update the map BEFORE the character since it manipulates the player's position
		currentMap->update(player, frameTime);
		player.update(frameTime);

	} else if(gameMenu.getActive()) { // if game menu is open
		gameMenu.update(frameTime);
	} else if(currentBattle) { // if in battle
		
		currentBattle->update(frameTime);
		if(input->wasKeyPressed('B')) {
			battleEnd();
		}
	}

	// Insta-quit with Q. Definitely don't want to keep this
	if(input->wasKeyPressed('Q')) {
		PostQuitMessage(0);
	}

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
	NPC* curNPC = currentMap->getFirstNPC();

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
		while(curNPC) {
			if(player.collidesWith((*curNPC), collisionVector)) {
				if(input->wasKeyPressed(gameConfig->getActionKey())) {
					if(!curNPC->getSpeaking())
						curNPC->setSpeaking(true); // start speaking to NPC with action key
					else  
						curNPC->progressSpeech(); // if already speaking, progress to the next message
				}
			//	if(input->wasKeyPressed(gameConfig->getCancelKey()))
			//		curNPC->setSpeaking(false); // always cancel speaking if cancel key pressed
			}
			curNPC = curNPC->getNextNPC();
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


    graphics->spriteBegin();                // begin drawing sprites
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Can't draw sprites outside this area ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw from bottom to top
	// if not in battle and currentMap is initialized
	if(!currentBattle && currentMap) {
		currentMap->render(&player);		

		// print x,y position on tiles if debug option is enabled
		if(currentMap->getTileNumbers())
			currentMap->drawTileNumbers();
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
	
	//Draw new Items if in a battle
	if(currentBattle){
		currentBattle->render();
	}
	// draw game menu if open
	if(gameMenu.getActive()) {
		gameMenu.render();
	}
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    graphics->spriteEnd();                  // end drawing sprites

	// draw collision boxes if enabled
	currentMap->collisionBoxes(graphics, &player);

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
		if(currentMap) {
			currentMap->setTileNumbers(!currentMap->getTileNumbers());               // toggle display of x,y tile position
			if(currentMap->getTileNumbers())
				console->print("Tile numbers on");
			else
				console->print("Tile numbers off");
		} else
			console->print("No map loaded");
	}

	if(command == "cb" || command == "collisionBoxes")
	{
		if(currentMap) {
			// toggle display of object collision boxes (not tiles)
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::PLAYER_MASK | mapNS::CHANGER_MASK | mapNS::CHEST_MASK | mapNS::NPC_MASK | mapNS::DOOR_MASK));
			console->print("Non-tile collision boxes swapped");
		} else
			console->print("No map loaded");
	}

	if(command == "cb tile")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::TILE_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::TILE_MASK)
				console->print("Tile collision boxes on");
			else
				console->print("Tile collision boxes off");
		} else
			console->print("No map loaded");
	}
	if(command == "cb player")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::PLAYER_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::PLAYER_MASK)
				console->print("Player collision boxes on");
			else
				console->print("Player collision boxes off");
		} else
			console->print("No map loaded");
	}
	if(command == "cb changer")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::CHANGER_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::CHANGER_MASK)
				console->print("Changer collision boxes on");
			else
				console->print("Changer collision boxes off");
		} else
			console->print("No map loaded");
	}
	if(command == "cb npc")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::NPC_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::NPC_MASK)
				console->print("NPC collision boxes on");
			else
				console->print("NPC collision boxes off");
		} else
			console->print("No map loaded");
	}
	if(command == "cb chest")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::CHEST_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::CHEST_MASK)
				console->print("Chest collision boxes on");
			else
				console->print("Chest collision boxes off");
		} else
			console->print("No map loaded");
	}
	if(command == "cb door")
	{
		if(currentMap) {
			currentMap->setCollsionBoxMask(currentMap->getCollisionBoxMask() ^ (mapNS::DOOR_MASK));
			if(currentMap->getCollisionBoxMask() & mapNS::DOOR_MASK)
				console->print("Door collision boxes on");
			else
				console->print("Door collision boxes off");
		} else
			console->print("No map loaded");
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
//			NPC* redGuy = currentMap->addNPC();

			if(!topChanger->initialize(TEST_MAP,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing graveyard changer 1"));

			if(!bottomChanger->initialize(TEST_MAP2,this,0, 0, 0, &changerTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing graveyard changer 2"));

//			if(!redGuy->initialize(this,34,34,2,&redCharTexture))
//				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing redGuy"));

			currentMap->setForeground(graphics, "pictures/graveyard2.0/fogforeground.png");
			currentMap->getForeground()->setAutoVscroll(-100.0);

			topChanger->setStartingPos(8,0); // top center of graveyard
			topChanger->setDestinationStartingPos(3,5);
			bottomChanger->setStartingPos(8,16); // bottom center of graveyard
			bottomChanger->setDestinationStartingPos(6,6);
//			redGuy->setStartingPos(8,7);
//			redGuy->setMoseyEndingPos(10,10);
//			redGuy->setCurrentFrame(0);

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

			if(!redGuy->initialize(this, 34, 34, 2, &redCharTexture, &dialogBox))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing redGuy"));

			if(!treasure->initialize(this, &chestTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing treasure"));

			if(!door->initialize(this,32,32,2, &doorTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing door"));

			currentMap->setBackground(graphics, "pictures/graveyard2.0/fogforeground.png");
			currentMap->getBackground()->setParallax(0.5);

			pitChanger->setStartingPos(8,2);
			pitChanger->setDestinationStartingPos(10,10);

			bottomChanger->setStartingPos(8,17); // bottom center of graveyard
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

			currentMap->setForeground(graphics, "pictures/graveyard2.0/fogforeground.png");
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

			currentMap->setForeground(graphics, "pictures/graveyard2.0/fogforeground.png");
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
	chestTexture.onLostDevice();
	doorTexture.onLostDevice();
	dialogBoxTexture.onLostDevice();
	gameMenu.onLostDevice();
	if(currentBattle)
		currentBattle->onLostDevice();
    Game::releaseAll();
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
	chestTexture.onResetDevice();
	doorTexture.onResetDevice();
	dialogBoxTexture.onResetDevice();
	gameMenu.onResetDevice();
	if(currentBattle)
		currentBattle->onResetDevice();
    Game::resetAll();
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

//=============================================================================
// Initiate battle
//=============================================================================
void WichitaGame::battleStart(const char* backgroundPic)
{
	SAFE_DELETE(currentBattle); // if it exists
	currentBattle = new Battle;

	printf("Battle Started\n");

	//Initialize the battle
	if(!currentBattle->initialize(this, backgroundPic, NULL, "pictures/battle/rainTest.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle"));

//	currentBattle->getBackground()->setAutoHscroll(-100.0);
//	currentBattle->getBackground()->setAutoVscroll(-70.0);

	currentBattle->getForeground()->setAutoHscroll(2000.0);
	currentBattle->getForeground()->setAutoVscroll(2000.0);
//	battleOn = true;
}

void WichitaGame::battleEnd()
{
//	battleOn = false;
	SAFE_DELETE(currentBattle);
}