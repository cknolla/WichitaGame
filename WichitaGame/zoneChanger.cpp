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
	active = true;
	edge = mapNS::TILE_COLLISION_BOX;
	return (Entity::initialize(gamePtr, width, height, ncols, textureM));
}

