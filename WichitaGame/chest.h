#ifndef _CHEST_H              // prevent multiple definitions if this 
#define _CHEST_H              // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

class Chest : public Entity
{
private:
	Chest* nextChest;
public:
	Chest();
	~Chest();

	bool initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM);

	// next item in linked list
	Chest* getNextChest() { return nextChest; }

	// set next chest in list
	void setNextChest(Chest* c) { nextChest = c; }
};

#endif