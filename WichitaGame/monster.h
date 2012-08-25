// Monster

#ifndef _MONSTER_H                // prevent multiple definitions if this 
#define _MONSTER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace monsterNS 
{
	const float MOVE_SPEED = 200.0f;
	const RECT COLLISION_BOX = {-8, 0, 8, 16};
}

class Monster : public Entity
{
private:
	char name[25];

public:
	Monster();
	virtual ~Monster();

	void update(float frameTime);

};

#endif
