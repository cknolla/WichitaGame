
#include "door.h"

Door::Door()
{
	nextDoor = NULL;
}

Door::~Door()
{
}

bool Door::initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	bool result = false;
	// initialize parent classes
	result = Entity::initialize(gamePtr, width, height, ncols, textureM);
	// set it active so that it can be collided with
	active = true;
	// collision box is the same size as its image
	edge.left = -spriteData.width/2;
	edge.right = spriteData.width/2;
	edge.top = -spriteData.height;
	edge.bottom = spriteData.height;
	
	return result;
}
