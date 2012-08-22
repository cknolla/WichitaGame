// Tile class

#ifndef _TILE_H
#define _TILE_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "text.h"

class Tile : public Entity
{
private:
	Tile* nextTile;
	int count;
public:
	Tile();
	virtual ~Tile();

	// Tiles will use the full texture provided. To change texture, reinitialize it
	bool initialize(Game* gamePtr, TextureManager* texture);

	// Get next tile in linked list
	Tile* getNextTile() { return nextTile; }

	// Set next tile in linked list
	void setNextTile(Tile* nt );
	void setCount(int c) { count = c; }
};

#endif