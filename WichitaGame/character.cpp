
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
	setX(Image::getX() + frameTime * characterNS::MOVE_SPEED);
	flipHorizontal(false);
}

void Character::moveLeft(float frameTime)
{
	setX(getX() - frameTime * characterNS::MOVE_SPEED);
	flipHorizontal(true);
}

void Character::moveUp(float frameTime)
{
	setY(getY() - frameTime * characterNS::MOVE_SPEED);
}

void Character::moveDown(float frameTime)
{
	setY(getY() + frameTime * characterNS::MOVE_SPEED);
}