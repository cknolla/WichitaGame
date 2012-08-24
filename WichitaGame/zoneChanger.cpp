// Zone Changer

#include "zoneChanger.h"

ZoneChanger::ZoneChanger()
{
	nextChanger = NULL;
	destStartX = 0.0;
	destStartY = 0.0;
}

ZoneChanger::~ZoneChanger()
{
}

bool ZoneChanger::initialize(MAP_LIST dest, Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	bool result = false;
	result = Entity::initialize(gamePtr, width, height, ncols, textureM);
	destination = dest;
	// set it active so that it can be collided with
	active = true;
	// collision box is the same size as its image
	edge.left = -spriteData.width/2;
	edge.right = spriteData.width/2;
	edge.top = -spriteData.height/2;
	edge.bottom = spriteData.height/2;
	// initialize parent classes
	return result;
}

void ZoneChanger::setDestinationStartingPos(int tileX, int tileY)
{
	destStartX = (float)tileX*TILE_WIDTH;
	destStartY = (float)tileY*TILE_HEIGHT;    

}

