
#include "monster.h"

Monster::Monster() : Entity()
{
}

Monster::~Monster()
{
}


void Monster::update(float frameTime)
{
	Image::update(frameTime);
	// update monster's previous location based on previous frame and new location based on current velocity
	setPrevLoc(spriteData.x, spriteData.y);
	spriteData.x += frameTime * velocity.x;
	spriteData.y += frameTime * velocity.y;
}
