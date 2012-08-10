
#include "character.h"

Character::Character() : Entity()
{
}

Character::~Character()
{
}

bool Character::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	 return (Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Character::moveRight(float frameTime) 
{
	setFrames(2,3);
	setX(Image::getX() + frameTime * characterNS::MOVE_SPEED);
	flipHorizontal(false);
//	setAnimationComplete(true);
}

void Character::moveLeft(float frameTime)
{
	setFrames(2,3);
	setX(getX() - frameTime * characterNS::MOVE_SPEED);
	flipHorizontal(true);
//	setAnimationComplete(true);
}

void Character::moveUp(float frameTime)
{
	setFrames(4,5);
	setY(getY() - frameTime * characterNS::MOVE_SPEED);
//	setAnimationComplete(true);
}

void Character::moveDown(float frameTime)
{
	setFrames(0,1);
//	setAnimationComplete(true);
	setY(getY() + frameTime * characterNS::MOVE_SPEED);
}