
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

void Character::update(float frameTime)
{
	Image::update(frameTime);
	setPrevLoc(spriteData.x, spriteData.y);
	spriteData.x += frameTime * velocity.x;
	spriteData.y += frameTime * velocity.y;
}

void Character::moveRight(float frameTime) 
{
	setFrames(2,3);
	flipHorizontal(false);
//	setPrevLoc(getX(), getY());
	setVelocity(VECTOR2(characterNS::MOVE_SPEED, velocity.y));
//	setX(Image::getX() + frameTime * characterNS::MOVE_SPEED);
}

void Character::moveLeft(float frameTime)
{
	setFrames(2,3);
//	setPrevLoc(getX(), getY());
//	setX(getX() - frameTime * characterNS::MOVE_SPEED);
	setVelocity(VECTOR2(-characterNS::MOVE_SPEED, velocity.y));
	flipHorizontal(true);
}

void Character::moveUp(float frameTime)
{
	setFrames(4,5);
//	setPrevLoc(getX(), getY());
//	setY(getY() - frameTime * characterNS::MOVE_SPEED);
	setVelocity(VECTOR2(velocity.x,-characterNS::MOVE_SPEED));
}

void Character::moveDown(float frameTime)
{
	setFrames(0,1);
//	setPrevLoc(getX(), getY());
//	setY(getY() + frameTime * characterNS::MOVE_SPEED);
	setVelocity(VECTOR2(velocity.x,characterNS::MOVE_SPEED));
}

void Character::stopX()
{
	setVelocity(VECTOR2(0.0f, velocity.y));
}

void Character::stopY()
{
	setVelocity(VECTOR2(velocity.x, 0.0f));
}
