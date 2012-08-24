
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
	// collision box extends below the image so that it opens about when the player's head hits it
	edge.left = -spriteData.width/2;
	edge.right = spriteData.width/2;
	edge.top = -spriteData.height/2;
	edge.bottom = spriteData.height;
	
	return result;
}
