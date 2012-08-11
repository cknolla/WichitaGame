// Character

#ifndef _CHARACTER_H                // prevent multiple definitions if this 
#define _CHARACTER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace characterNS 
{
	const float MOVE_SPEED = 150.0f;
	const RECT COLLISION_BOX = {-12, 0, 12, 14};
}

class Character : public Entity
{
private:
	char name[25];
	float prevX;
	float prevY;

public:
	Character();
	virtual ~Character();

	bool initialize(Game *gamePtr, int width, int height, int ncols,TextureManager *textureM);
	void update(float frameTime);

	void moveRight(float frameTime);
	void moveLeft(float frameTime);
	void moveUp(float frameTime);
	void moveDown(float frameTime);
	void stopX();
	void stopY();

	float getPrevX() { return prevX; }
	void setPrevX(float newPrevX) { prevX = newPrevX; }
	float getPrevY() { return prevY; }
	void setPrevY(float newPrevY) { prevY = newPrevY; }
	void setPrevLoc(float newPrevX, float newPrevY) { prevX = newPrevX; prevY = newPrevY; }

};

#endif
