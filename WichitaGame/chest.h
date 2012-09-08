#ifndef _CHEST_H              // prevent multiple definitions if this 
#define _CHEST_H              // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "text.h"

class Chest : public Entity
{
private:
	bool opened;

	Chest* nextChest;
public:
	Chest();
	~Chest();

	// initialize with standard image properties as well as game's generic dialog box and text
	bool initialize(Game* gamePtr, TextureManager* textureM, int width = TILE_WIDTH, int height = TILE_HEIGHT, int ncols = 2);

	// next item in linked list
	Chest* getNextChest() { return nextChest; }

	// set next chest in list
	void setNextChest(Chest* c) { nextChest = c; }

	// open the chest if it isn't yet
	void open();
};

#endif