#ifndef _CHEST_H              // prevent multiple definitions if this 
#define _CHEST_H              // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

class Chest : public Entity
{
private:
public:
	Chest();
	~Chest();
};

#endif