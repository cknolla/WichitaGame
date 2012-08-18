// Map class

#include "map.h"


Map::Map()
{
	initialized = false;
	firstTile = NULL;
	firstTexture = NULL;
	firstChanger = NULL;
	firstNPC = NULL;
}

Map::~Map()
{
	unload();
}

bool Map::initialize(Game* gamePtr, const char* tileSet[], const char* keyFile)
{
	std::ifstream key(keyFile);
	std::ofstream debugFile("mapDebug.txt");
	char formatStr[30]; // used to verify map file format
	char errorStr[200];
	int curKey;
	int collidables;
	int row, col;
	int startXTile, startYTile;
	int i;
	TextureManager* curTexture = NULL;
	TextureManager* prevTexture = NULL;
	TextureManager* useTexture = NULL;
	Tile* curTile = NULL;
	Tile* prevTile = NULL;
//	try {
	// check if key file opened
	if(!key.is_open()) {
		sprintf_s(errorStr, "Could not open map file %s", keyFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		return false;
	}

	if(!debugFile.is_open()) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening map debug file"));
		return false;
	}

	// file should be formatted as follows:
	// StartingTile: playerStartX playerStartY
	// WidthHeight: width height
	// row1
	// row2
	// row(height)
	// Collidables: 
	// row1
	// row2
	// row(height)

	// tile map texture
	for(i = 0; i < sizeof(tileSet); i++) {
		curTexture = new TextureManager;
		if(!firstTexture)
			firstTexture = curTexture;
		if(prevTexture)
			prevTexture->setNextTexture(curTexture);
		sprintf_s(errorStr, "Error initializing map texture %s", tileSet[i]);
		if (!curTexture->initialize(gamePtr->getGraphics(),tileSet[i])) {
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		prevTexture = curTexture;
	}
	
	key >> formatStr;
//		debugFile << formatStr << "\n";
	if(strcmp(formatStr, "StartingTile:")) { // strcmp returns non-zero (true) if there are any differences
		sprintf_s(errorStr, "'StartingTile:' does not exist or is not correct at the start of %s! Can't load map", keyFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		return false;
	}
	key >> startXTile >> startYTile;
	startX = (float)startXTile*mapNS::TILE_WIDTH;
	startY = (float)startYTile*mapNS::TILE_HEIGHT;

	key >> formatStr;
//		debugFile << formatStr << "\n";
	if(strcmp(formatStr, "WidthHeight:")) {
		sprintf_s(errorStr, "The 'WidthHeight:' string does not exist or does not match below TopLeftTile: in the map file %s! Can't load map", keyFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		return false;
	}
	key >> width >> height;

	// create all the tiles, assign them a texture, and link them in a linked list
	for(row = 0; row < height; row++) { 
		for(col = 0; col < width; col++) {
			curTile = new Tile;
			if(!firstTile)
				firstTile = curTile;
			if(prevTile)
				prevTile->setNextTile(curTile);
			key >> curKey; // grab the texture number to use for the new tile
			useTexture = firstTexture;
			// flip through the texture list to select the correct one for this tile
			for(i = 0; i < curKey; i++)
				useTexture = useTexture->getNextTexture();
			// initialize each tile
			if (!curTile->initialize(gamePtr,useTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
			// assign the approriate sprite for this tile using the key (always 0)
			curTile->setCurrentFrame(0);
			// place the tile where it belongs on screen - center the character
			curTile->setX( (float)mapNS::TILE_WIDTH * col - startX + GAME_WIDTH/2);
			curTile->setY( (float)mapNS::TILE_HEIGHT * row - startY + GAME_HEIGHT/2);

			// give it full-box collision
			curTile->setEdge(mapNS::TILE_COLLISION_BOX);

			prevTile = curTile;
		}
//			debugFile << "\n";
	}
	key >> formatStr;
	// make collidables large enough to hold every tile number in the texture
	if(strcmp(formatStr, "Collidables:")) {
		sprintf_s(errorStr, "The 'Collidables:' string does not exist, is not correct, or your map file's grid width and height aren't correct! Can't load map", keyFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
		return false;
	}
	
	curTile = firstTile;
	while(curTile) {
		// pull in the collision map. If it's a non-zero number, that tile will be collidable (active)
		if(key.eof()) {
			sprintf_s(errorStr, "Warning: collision map size does match tile map size in %s!", keyFile);
			throw(GameError(gameErrorNS::WARNING, errorStr));
		}
		key >> collidables;
		if(collidables == 1)
			curTile->setActive(true);
		else
			curTile->setActive(false);
		curTile->setLayer(collidables);
		curTile = curTile->getNextTile();
	}
	key.close();
	debugFile.close();
		
//	} catch(...) {
//		return false;
//	}
	initialized = true;
	return initialized;
}

void Map::update(Character &player, float frameTime)
{
	int shiftLeft = 0;
	int shiftRight = 0;
	int shiftUp = 0;
	int shiftDown = 0;
	float playerCenterX = player.getX() + player.getWidth()/2;
	float playerCenterY = player.getY() + player.getHeight()/2;
	Tile* curTile = firstTile;
	ZoneChanger* curChanger = firstChanger;
	NPC* curNPC = firstNPC;

	if(playerCenterX + player.getEdge().left < mapNS::CAMERA_TRIGGER) {
		// if the player's collision box passes the 'trigger' point, scroll the map rather than the player
		shiftLeft = 1;
	} else if(playerCenterX + player.getEdge().right > GAME_WIDTH - mapNS::CAMERA_TRIGGER) {
		shiftRight = 1;
	}
	if(playerCenterY + player.getEdge().top < mapNS::CAMERA_TRIGGER) {
		shiftUp = 1;
	} else if(playerCenterY + player.getEdge().bottom > GAME_HEIGHT - mapNS::CAMERA_TRIGGER) {
		shiftDown = 1;
	}

	// move each tile rather than the player to scroll the map
	while(curTile) {
		if(shiftLeft)
			curTile->setX( curTile->getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftRight)
			curTile->setX( curTile->getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftUp)
			curTile->setY( curTile->getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftDown)
			curTile->setY( curTile->getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		curTile = curTile->getNextTile();
	}

	// move each object on the map
	while(curChanger) {
		if(shiftLeft)
			curChanger->setX( curChanger->getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftRight)
			curChanger->setX( curChanger->getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftUp)
			curChanger->setY( curChanger->getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftDown)
			curChanger->setY( curChanger->getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		curChanger = curChanger->getNextChanger();
	}
	while(curNPC) {
		if(shiftLeft)
			curNPC->setX( curNPC->getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftRight)
			curNPC->setX( curNPC->getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftUp)
			curNPC->setY( curNPC->getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
		if(shiftDown)
			curNPC->setY( curNPC->getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
		curNPC = curNPC->getNextNPC();
	}

	// push the player back by an equal amount that the camera moved. This keeps the player always on screen
	if(shiftLeft)
		player.setX( player.getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
	if(shiftRight)
		player.setX( player.getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED));
	if(shiftUp)
		player.setY( player.getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED));
	if(shiftDown)
		player.setY( player.getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED));

}

void Map::collisions(Character& player)
{
	VECTOR2 collisionVector;
	float tempX;
	float tempY;
	bool Xoffender = false;
	bool Yoffender = false;
	Tile* curTile = firstTile;

	while(curTile) {
		if(player.collidesWith(*curTile, collisionVector)) {
		//	sprintf_s(debugLineBuf, "Collision!");
			// save the destination location
			tempX = player.getX();
			tempY = player.getY();
			// place the character back on the X axis
			player.setX(player.getPrevX());
			if(player.collidesWith(*curTile, collisionVector)) {
				// if there is still collision after placing him back where he came from on the X axis, then Y axis is offending
				Yoffender = true;
			}
			player.setX(tempX); // put him back to new position to check Y
			player.setY(player.getPrevY()); // return him to previous Y to see if X is offending
			if(player.collidesWith(*curTile, collisionVector)) {
				// if there is still collision after placing him back where he came from on the Y axis, then X axis is offending
				Xoffender = true;
			}
			player.setY(tempY); // put him in new position
			if(Xoffender && !Yoffender) {
				player.setX(player.getPrevX()); // allow Y movement since Y didn't cause collision
			} else if(Yoffender && !Xoffender) {
				player.setY(player.getPrevY()); // allow X movement since X didn't cause collision
			} else if(Xoffender && Yoffender) {
				player.setX(player.getPrevX());
				player.setY(player.getPrevY()); // don't allow movement in either direction (corner)
			}	
		}
		curTile = curTile->getNextTile();
	}
}

void Map::reset()
{
	int row, col;
	Tile* curTile = firstTile;
	ZoneChanger* curChanger = firstChanger;
	NPC* curNPC = firstNPC;
	if(initialized) {
		// reset tiles to starting location
		while(curTile) {
			for(row = 0; row < height; row++) { 
				for(col = 0; col < width; col++) {
					curTile->setX( (float)mapNS::TILE_WIDTH * col - startX + GAME_WIDTH/2);
					curTile->setY( (float)mapNS::TILE_HEIGHT * row - startY + GAME_HEIGHT/2);
					curTile = curTile->getNextTile();
				}
			}
		}
		// reset map objects to starting location
		while(curChanger) {
			curChanger->setX(curChanger->getStartX() - startX + GAME_WIDTH/2);
			curChanger->setY(curChanger->getStartY() - startY + GAME_HEIGHT/2);
			curChanger = curChanger->getNextChanger();
		}
		while(curNPC) {
			curNPC->setX(curNPC->getStartX() - startX + GAME_WIDTH/2);
			curNPC->setY(curNPC->getStartY() - startY + GAME_HEIGHT/2);
			curNPC = curNPC->getNextNPC();
		}
	}
}

void Map::setStartingPos(int tileX, int tileY)
{
	startX = (float)tileX*mapNS::TILE_WIDTH;
	startY = (float)tileY*mapNS::TILE_HEIGHT;    

}

void Map::getXY(float & x , float & y , int tileX , int tileY ){
	x = (float)tileX*mapNS::TILE_WIDTH;
	y = (float)tileY*mapNS::TILE_HEIGHT;  
}

ZoneChanger* Map::addChanger()
{
	ZoneChanger* newChanger = new ZoneChanger;
	ZoneChanger* curChanger = firstChanger;
	ZoneChanger* prevChanger = NULL;
	if(!firstChanger)
		firstChanger = newChanger;
	else {
		while(curChanger) {
			prevChanger = curChanger;
			curChanger = curChanger->getNextChanger();
		}
		if(prevChanger)
			prevChanger->setNextChanger(newChanger);
	}
	return newChanger;
}

NPC* Map::addNPC()
{
	NPC* newNPC = new NPC;
	NPC* curNPC = firstNPC;
	NPC* prevNPC = NULL;
	if(!firstNPC)
		firstNPC = newNPC;
	else {
		while(curNPC) {
			prevNPC = curNPC;
			curNPC = curNPC->getNextNPC();
		}
		if(prevNPC)
			prevNPC->setNextNPC(newNPC);
	}
	return newNPC;
}


void Map::onLostDevice()
{
	TextureManager* curTexture = firstTexture;
	while(curTexture) {
		curTexture->onLostDevice();
		curTexture = curTexture->getNextTexture();
	}
}

void Map::onResetDevice()
{
	TextureManager* curTexture = firstTexture;
	while(curTexture) {
		curTexture->onResetDevice();
		curTexture = curTexture->getNextTexture();
	}
}

void Map::unload()
{
	Tile* curTile = firstTile;
	Tile* nextTile;
	TextureManager* curTexture = firstTexture;
	TextureManager* nextTexture;
	ZoneChanger* curChanger = firstChanger;
	ZoneChanger* nextChanger;
	NPC* curNPC = firstNPC;
	NPC* nextNPC;

	onLostDevice();
	// delete the full linked list of tiles and textures
	while(curTile) {
		nextTile = curTile->getNextTile();
		SAFE_DELETE(curTile);
		curTile = nextTile;
	}
	while(curTexture) {
		nextTexture = curTexture->getNextTexture();
		SAFE_DELETE(curTexture);
		curTexture = nextTexture;
	}
	while(curChanger) {
		nextChanger = curChanger->getNextChanger();
		SAFE_DELETE(curChanger);
		curChanger = nextChanger;
	}
	while(curNPC) {
		nextNPC = curNPC->getNextNPC();
		SAFE_DELETE(curNPC);
		curNPC = nextNPC;
	}
}