
#include "character.h"

Character::Character() : Entity()
{
}

Character::~Character()
{
}

//bool Character::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
//{
//	return (Entity::initialize(gamePtr, width, height, ncols, textureM));
//}

void Character::update(float frameTime)
{
	Image::update(frameTime);
	// update player's previous location based on previous frame and new location based on current velocity
	setPrevLoc(spriteData.x, spriteData.y);
	spriteData.x += frameTime * velocity.x;
	spriteData.y += frameTime * velocity.y;
}

void Character::moveRight(float frameTime) 
{
	// select frames of animation for rightward movement
	setFrames(2,3);
	flipHorizontal(false);
	setVelocity(VECTOR2(characterNS::MOVE_SPEED, velocity.y));
}

void Character::moveLeft(float frameTime)
{
	// select frames of animation for leftward movement
	setFrames(2,3);
	setVelocity(VECTOR2(-characterNS::MOVE_SPEED, velocity.y));
	flipHorizontal(true);
}

void Character::moveUp(float frameTime)
{
	// select frames of animation for upward movement
	setFrames(4,5);
	setVelocity(VECTOR2(velocity.x,-characterNS::MOVE_SPEED));
}

void Character::moveDown(float frameTime)
{
	// select frames of animation for downward movement
	setFrames(0,1);
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
