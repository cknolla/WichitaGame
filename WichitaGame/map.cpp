// Map class

#include "map.h"


Map::Map()
{
	initialized = false;
}

Map::~Map()
{
	onLostDevice();
}

bool Map::initialize(Game* gamePtr, const char* textureFile, const char* keyFile)
{
	std::ifstream key(keyFile);
	std::ofstream debugFile("mapDebug.txt");
	char formatStr[30]; // used to verify map file format
	char errorStr[200];
	int curKey;
	int collidables;
	int row, col;
	int startXTile, startYTile;
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
		// Collidables: collidable1 collidable2 collidable(n)

		// tile map texture
		sprintf_s(errorStr, "Error initializing map texture %s", textureFile);
		if (!mapTexture.initialize(gamePtr->getGraphics(),textureFile)) {
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		key >> formatStr;
		debugFile << formatStr << "\n";
		if(strcmp(formatStr, "StartingTile:")) { // strcmp returns non-zero (true) if there are any differences
			sprintf_s(errorStr, "'StartingTile:' does not exist or is not correct at the start of %s! Can't load map", keyFile);
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		key >> startXTile >> startYTile;
		startX = (float)startXTile*mapNS::TILE_WIDTH;
		startY = (float)startYTile*mapNS::TILE_HEIGHT;

//		key >> formatStr;
//		debugFile << formatStr << "\n";
//		if(strcmp(formatStr, "TopLeftTile:")) {
//			sprintf_s(errorStr, "The 'TopLeftTile:' string does not exist or does not match below StartingTile: in the map file %s! Can't load map", keyFile);
//			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
//			return false;
//		}
//		key >> xOffset >> yOffset;
//		xOffset *= mapNS::TILE_WIDTH;
//		yOffset *= mapNS::TILE_HEIGHT;

//		startX -= xOffset;
//		startY -= yOffset;

		key >> formatStr;
		debugFile << formatStr << "\n";
		if(strcmp(formatStr, "WidthHeight:")) {
			sprintf_s(errorStr, "The 'WidthHeight:' string does not exist or does not match below TopLeftTile: in the map file %s! Can't load map", keyFile);
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		key >> width >> height;
		if(width > mapNS::MAX_MAP_WIDTH) {
			sprintf_s(errorStr, "Width of map %s (%d) is larger than maximum allowed (%d)!", keyFile, width, mapNS::MAX_MAP_WIDTH);
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		if(height > mapNS::MAX_MAP_HEIGHT) {
			sprintf_s(errorStr, "Height of map %s (%d) is larger than maximum allowed (%d)!", keyFile, height, mapNS::MAX_MAP_HEIGHT);
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}

//		debugFile << width << " " << height << "\n";

		for(row = 0; row < height; row++) { 
			for(col = 0; col < width; col++) {
				// initialize image for each tile
				if (!tile[row][col].initialize(gamePtr,mapNS::TILE_WIDTH,mapNS::TILE_HEIGHT,0,&mapTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
				key >> curKey;
//				debugFile << curKey << " ";
				// assign the approriate sprite for this tile using the key
				tile[row][col].setCurrentFrame(curKey);
				// place the tile where it belongs on screen - center the character
				tile[row][col].setX( (float)mapNS::TILE_WIDTH * col - startX + GAME_WIDTH/2);
				tile[row][col].setY( (float)mapNS::TILE_HEIGHT * row - startY + GAME_HEIGHT/2);
				// need an array defining collidable
				tile[row][col].setEdge(mapNS::TILE_COLLISION_BOX);
			}
//			debugFile << "\n";
		}
		key >> formatStr;
		// make collidables large enough to hold every tile number in the texture
		if(strcmp(formatStr, "Collidables:")) {
			sprintf_s(errorStr, "The 'Collidables:' string does not exist or is not correct at the bottom of %s! Can't load map", keyFile);
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
			return false;
		}
		while(!key.eof()) {
			key >> collidables;
			for(row = 0; row < height; row++) {
				for(col = 0; col < width; col++) {
					if(tile[row][col].getCurrentFrame() == collidables) { // if it's in the list of collidables
						tile[row][col].setActive(true);
					}
				}
			}
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

	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col++) {
			// shift each direction only if the appropriate variable doesn't zero it out
			tile[row][col].setX( tile[row][col].getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftLeft));

			tile[row][col].setX( tile[row][col].getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftRight));
			
			tile[row][col].setY( tile[row][col].getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftUp));
			
			tile[row][col].setY( tile[row][col].getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftDown));
			
		}
	}

	// push the player back by an equal amount that the camera moved. This keeps the player always on screen
	player.setX( player.getX() + (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftLeft));
	player.setX( player.getX() - (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftRight));
	player.setY( player.getY() + (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftUp));
	player.setY( player.getY() - (frameTime * mapNS::CAMERA_MOVE_SPEED * shiftDown));
/* FAILING METHOD OF PUSHING THE PLAYER BACK
	if(shiftLeft || shiftRight) {
//		tempPrevX = player.getPrevX();
		player.setX(player.getX()-(frameTime*player.getVelocity().x*2));
//		player.setPrevX(player.getPrevX()+player.getVelocity().x*frameTime);
//		player.setVelocity(VECTOR2(0, player.getVelocity().y));
		
	}

	if(shiftUp || shiftDown) {
		player.setY(player.getY()-(frameTime*player.getVelocity().y*2));
//		player.setPrevY(player.getPrevY()+player.getVelocity().y*frameTime);
//		player.setVelocity(VECTOR2(player.getVelocity().x, 0));
		
	}

*/
}

void Map::reset()
{
	int row, col;
	if(initialized) {
		for(row = 0; row < height; row++) { 
			for(col = 0; col < width; col++) {
				tile[row][col].setX( (float)mapNS::TILE_WIDTH * col - startX + GAME_WIDTH/2);
				tile[row][col].setY( (float)mapNS::TILE_HEIGHT * row - startY + GAME_HEIGHT/2);
			}
		}
	}
}

void Map::setStartingPos(int tileX, int tileY)
{
	startX = tileX*mapNS::TILE_WIDTH;
	startY = tileY*mapNS::TILE_HEIGHT;
}

Entity* Map::getTile(int row, int col)
{
	if(initialized)
		return &tile[row][col];
	else
		return NULL;
}

void Map::onLostDevice()
{
	if(initialized)
		mapTexture.onLostDevice();
}

void Map::onResetDevice()
{
	if(initialized)
		mapTexture.onResetDevice();
}