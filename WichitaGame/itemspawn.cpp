#include "itemspawn.h"
#include "textureManager.h"


ItemSpawn::ItemSpawn()
{
	textureManager = NULL;
}


ItemSpawn::~ItemSpawn()
{
	//if(textureManager != NULL )
	//	delete textureManager;
}


bool ItemSpawn::initialize(Map* dest, Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{

	destination = dest;
	active = true;
	edge = mapNS::TILE_COLLISION_BOX;
	return (Entity::initialize(gamePtr, width, height, ncols, textureM));
	//textureManager = textureM;
}


