// Zone changer

#ifndef
#define

#include "entity.h"

class ZoneChanger : public Entity
{
private:
	Map* destination;
public:
	ZoneChanger();
	~ZoneChanger();

	void intitialize(Map* dest);

};

#endif