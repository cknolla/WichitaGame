// Tile Class

#include "tile.h"
#include <stdio.h>

Tile::Tile()
{
	nextTile = NULL;
	count = 0;
}

Tile::~Tile()
{
	//printf("Deleting %d\n" , count );
	//SAFE_DELETE(nextTile);
	//nextTile = NULL;
}

bool Tile::initialize(Game* gamePtr, TextureManager* texture)
{
	// Tiles will be in independent texture files, so width, height, and cols can be defaulted to 0
	return(Entity::initialize(gamePtr, 0, 0, 0, texture));
}


void Tile::setNextTile(Tile* nt )
{ 
	nextTile = nt;  
	nextTile->setCount(count+1);
}