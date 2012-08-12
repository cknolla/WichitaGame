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
	char errorStr[100];
	int curKey;
	int* collidables;
//	int collidablesSize;
//	int actualCollidables;
	int row, col;
	int startXTile, startYTile;
	try {
		// check if key file opened
		sprintf_s(errorStr, "Could not open map file %s", keyFile);
		if(!key.is_open())
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));

		if(!debugFile.is_open())
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening map debug file"));

		// file should be formatted as follows:
		// playerStartX playerStartY
		// width height
		// row1
		// row2
		// row(height)
		// collidable1 collidable2 collidable(n)

		// tile map texture
		sprintf_s(errorStr, "Error initializing map texture %s", textureFile);
		if (!mapTexture.initialize(gamePtr->getGraphics(),textureFile))
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));

		key >> startXTile >> startYTile;
		startX = (float)startXTile*mapNS::TILE_WIDTH;
		startY = (float)startYTile*mapNS::TILE_HEIGHT;
		debugFile << startXTile << startYTile << startX << startY;

		key >> width >> height;
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
				// place the tile where it belongs on screen
				tile[row][col].setY((float)mapNS::TILE_HEIGHT*row);
				tile[row][col].setX((float)mapNS::TILE_WIDTH*col);
				// need an array defining collidable
				tile[row][col].setEdge(mapNS::TILE_COLLISION_BOX);
			}
//			debugFile << "\n";
		}

		// make collidables large enough to hold every tile number in the texture

		while(!key.eof()) {
			collidables = (int*)malloc(sizeof(int));
			key >> *collidables;
			for(row = 0; row < height; row++) {
				for(col = 0; col < width; col++) {
					if(tile[row][col].getCurrentFrame() == *collidables) { // if it's in the list of collidables
						tile[row][col].setActive(true);
					}
				}
			}
			free(collidables);
		}
		key.close();
		debugFile.close();
		
	} catch(...) {
		return false;
	}
	initialized = true;
	return initialized;
}

void Map::update(Character &player, float frameTime)
{
	float x = player.getX();
	float y = player.getY();
	int shiftLeft = 0;
	int shiftRight = 0;
	int shiftUp = 0;
	int shiftDown = 0;

	if(x < mapNS::CAMERA_TRIGGER) {
		// if the player's position is 20% toward the screen's left, shift the map by the player's velocity
		shiftLeft = 1;
	} else if(x > GAME_WIDTH-mapNS::CAMERA_TRIGGER) {
		// if the player's position is 80% toward the screen's right, shift the map right
		shiftRight = 1;
	}
	if(y < mapNS::CAMERA_TRIGGER) {
		shiftUp = 1;
	} else if(y > GAME_HEIGHT-mapNS::CAMERA_TRIGGER) {
		shiftDown = 1;
	}

	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col++) {
			// shift each direction only if the appropriate variable doesn't zero it out
			tile[row][col].setX(tile[row][col].getX()+(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftLeft));

			tile[row][col].setX(tile[row][col].getX()-(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftRight));
			
			tile[row][col].setY(tile[row][col].getY()+(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftUp));
			
			tile[row][col].setY(tile[row][col].getY()-(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftDown));
			
		}
	}

	// push the player back by an equal amount that the camera moved. This keeps the player always on screen
	player.setX(player.getX()+(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftLeft));
	player.setX(player.getX()-(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftRight));
	player.setY(player.getY()+(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftUp));
	player.setY(player.getY()-(frameTime*mapNS::CAMERA_MOVE_SPEED*shiftDown));
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
	for(row = 0; row < height; row++) { 
			for(col = 0; col < width; col++) {
				tile[row][col].setY((float)mapNS::TILE_HEIGHT*row);
				tile[row][col].setX((float)mapNS::TILE_WIDTH*col);
			}
	}
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