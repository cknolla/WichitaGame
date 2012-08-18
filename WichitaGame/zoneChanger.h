// Zone changer

#ifndef _ZONECHANGER_H
#define _ZONECHANGER_H

#include "entity.h"

class ZoneChanger : public Entity
{
private:
	MAP_LIST destination;
	ZoneChanger* nextChanger;
public:
	ZoneChanger();
	~ZoneChanger();

	// only need destination for zoneChanger, but also have to initialize entity and image parents
	bool initialize(MAP_LIST dest, Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	MAP_LIST getDestination() { return destination; }

	// return next zone changer in linked list
	ZoneChanger* getNextChanger() { return nextChanger; }

	// set next zone changer in ZC list
	void setNextChanger(ZoneChanger* nz) { nextChanger = nz; }

};



#endif