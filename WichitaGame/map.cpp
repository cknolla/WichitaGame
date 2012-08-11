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
	try {
		// check if key file opened
		sprintf_s(errorStr, "Could not open map file %s", keyFile);
		if(!key.is_open())
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));

		if(!debugFile.is_open())
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening map debug file"));

		// file should be formatted as follows:
		// width height
		// row1
		// row2
		// row(height)
		// collidable1 collidable2 collidable(n)

		// tile map texture
		sprintf_s(errorStr, "Error initializing map texture %s", textureFile);
		if (!mapTexture.initialize(gamePtr->getGraphics(),textureFile))
			throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));

		key >> width >> height;
		debugFile << width << " " << height << "\n";

		for(row = 0; row < height; row++) { // rows
			for(col = 0; col < width; col++) { // cols
				// initialize image for each tile
				if (!tile[row][col].initialize(gamePtr,mapNS::TILE_WIDTH,mapNS::TILE_HEIGHT,0,&mapTexture))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
				key >> curKey;
				debugFile << curKey << " ";
				// assign the approriate sprite for this tile using the key
				tile[row][col].setCurrentFrame(curKey);
				// place the tile where it belongs on screen
				tile[row][col].setY((float)mapNS::TILE_HEIGHT*row);
				tile[row][col].setX((float)mapNS::TILE_WIDTH*col);
				// need an array defining collidable
				tile[row][col].setEdge(mapNS::TILE_COLLISION_BOX);
			}
			debugFile << "\n";
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