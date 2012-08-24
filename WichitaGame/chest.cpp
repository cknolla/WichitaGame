
#include "chest.h"

Chest::Chest()
{
	nextChest = NULL;
}

Chest::~Chest()
{
}

bool Chest::initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	bool result = false;
	// initialize parent classes
	result = Entity::initialize(gamePtr, width, height, ncols, textureM);
	// set it active so that it can be collided with
	active = true;
	// collision box is the same size as its image
	edge.left = -spriteData.width/2;
	edge.right = spriteData.width/2;
	edge.top = -spriteData.height/2;
	edge.bottom = spriteData.height/2;
	
	return result;
}

