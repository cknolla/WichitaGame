#ifndef _DOOR_H              // prevent multiple definitions if this 
#define _DOOR_H              // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

class Door : public Entity
{
private:
	Door* nextDoor;
public:
	Door();
	~Door();

	bool initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM);

	// next item in linked list
	Door* getNextDoor() { return nextDoor; }

	// set next Door in list
	void setNextDoor(Door* d) { nextDoor = d; }
};

#endif
