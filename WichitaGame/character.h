// Character

#ifndef _CHARACTER_H                // prevent multiple definitions if this 
#define _CHARACTER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace characterNS 
{
	const float MOVE_SPEED = 200.0f;
	const RECT COLLISION_BOX = {-8, 0, 8, 16};
}

class Character : public Entity
{
private:
	char name[25];
	// direction character is facing
	char directionUD;
	char directionLR;

public:
	Character();
	virtual ~Character();

//	bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);
	void update(float frameTime);

	void moveRight(float frameTime);
	void moveLeft(float frameTime);
	void moveUp(float frameTime);
	void moveDown(float frameTime);
	void stopX();
	void stopY();

	char getDirectionLR() { return directionLR; }
	char getDirectionUD() { return directionUD; }

};

#endif
