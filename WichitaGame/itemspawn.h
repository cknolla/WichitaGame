
#ifndef _ITEMSPAWN_H
#define _ITEMSPAWN_H

#include "entity.h"
#include "map.h"

class ItemSpawn : public Entity
{
private:
	//TextureManager *textureManager;
	Map* destination;
public:
	ItemSpawn();
	~ItemSpawn();

	bool initialize(Map* dest, Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	Map* getDestination() { return destination; }

};



#endif