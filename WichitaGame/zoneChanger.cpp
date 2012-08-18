// Zone Changer

#include "zoneChanger.h"

ZoneChanger::ZoneChanger()
{
	nextChanger = NULL;
}

ZoneChanger::~ZoneChanger()
{
}

bool ZoneChanger::initialize(const char* dest, Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	bool result = false;
	result = Entity::initialize(gamePtr, width, height, ncols, textureM);
	strcpy_s(destination, dest);
	//destination = dest;
	// set it active so that it can be collided with
	active = true;
	// collision box is the same size as a tile
	edge = spriteData.rect;
	// initialize parent classes
	return result;
}

