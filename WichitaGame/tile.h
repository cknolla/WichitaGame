// Tile class

#ifndef _TILE_H
#define _TILE_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

class Tile : public Entity
{
private:
	Tile* nextTile;
	int layer;
	int count;
public:
	Tile();
	virtual ~Tile();

	// Tiles will use the full texture provided. To change texture, reinitialize it
	bool initialize(Game* gamePtr, TextureManager* texture);

	// Get next tile in linked list
	Tile* getNextTile() { return nextTile; }

	// Get drawing layer
	int getLayer() { return layer; }

	// Set drawing layer
	void setLayer(int l) { layer = l; }

	// Set next tile in linked list
	void setNextTile(Tile* nt );
	void setCount(int c) { count = c; }
};

#endif