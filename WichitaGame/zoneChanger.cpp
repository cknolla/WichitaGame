// Zone Changer

#include "zoneChanger.h"

ZoneChanger::ZoneChanger()
{
}

ZoneChanger::~ZoneChanger()
{
}

bool ZoneChanger::initialize(Map* dest, Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	destination = dest;
	// set it active so that it can be collided with
	active = true;
	// collision box is the same size as a tile
	edge = mapNS::TILE_COLLISION_BOX;
	// initialize parent classes
	return (Entity::initialize(gamePtr, width, height, ncols, textureM));
}

