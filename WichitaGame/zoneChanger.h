// Zone changer

#ifndef _ZONECHANGER_H
#define _ZONECHANGER_H

#include "entity.h"
#include "map.h"

class ZoneChanger : public Entity
{
private:
	Map* destination;
public:
	ZoneChanger();
	~ZoneChanger();

	// only need destination for zoneChanger, but also have to initialize entity and image parents
	bool initialize(Map* dest, Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	Map* getDestination() { return destination; }

};



#endif