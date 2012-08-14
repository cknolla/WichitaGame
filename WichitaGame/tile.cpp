// Tile Class

#include "tile.h"

Tile::Tile()
{
	nextTile = NULL;
}

Tile::~Tile()
{
	SAFE_DELETE(nextTile);
}

bool Tile::initialize(Game* gamePtr, TextureManager* texture)
{
	// Tiles will be in independent texture files, so width, height, and cols can be defaulted to 0
	return(Entity::initialize(gamePtr, 0, 0, 0, texture));
}
