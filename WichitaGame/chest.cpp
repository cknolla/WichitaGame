
#include "chest.h"

Chest::Chest()
{
	opened = false;
	nextChest = NULL;
}

Chest::~Chest()
{
}

bool Chest::initialize(Game* gamePtr, TextureManager* textureM, int width, int height, int ncols)
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

void Chest::open()
{
	// UNDER CONSTRUCTION - NOT READY FOR USE
	if(opened)
		return;
	opened = true;
	char buffer[1000];
	// look up item in database to give to player
	setCurrentFrame(1); // chest open
	sprintf_s(buffer, "You receive 'Item X'!");


}

