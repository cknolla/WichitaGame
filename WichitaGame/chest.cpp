
#include "chest.h"

Chest::Chest()
{
	opened = false;
	locked = false;
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

bool Chest::update(Character& player)
{
	bool openOccurred = false;
	VECTOR2 collisionVector;
	//move chest right by 5 pixels to check for collision with player
	spriteData.x += 5.0f;
	// if chest now collides with player and he is facing left, open the chest
	if(collidesWith(player, collisionVector) && player.getDirectionLR() == 'L')
		openOccurred |= open();
	spriteData.x -= 5.0f; // return to original position
	spriteData.x -= 5.0f; // shift left by 5 to look for collision
	if(collidesWith(player, collisionVector) && player.getDirectionLR() == 'R')
		openOccurred |= open();
	spriteData.x += 5.0f; // return
	spriteData.y += 5.0f; // shift down by 5
	if(collidesWith(player, collisionVector) && player.getDirectionUD() == 'U')
		openOccurred |= open();
	spriteData.y -= 5.0f; // return
	spriteData.y -= 5.0f; // shift up by 5
	if(collidesWith(player, collisionVector) && player.getDirectionUD() == 'D')
		openOccurred |= open();
	spriteData.y += 5.0f; // return
	return openOccurred;
}

bool Chest::open()
{
	if(opened)
		return false;
	if(locked)
		return false;
	opened = true;
	char buffer[1000];
	// look up item in database to give to player
	setCurrentFrame(1); // chest open
	sprintf_s(buffer, "You receive 'Item X'!");
	return true;
}

