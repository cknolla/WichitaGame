// Zone changer

#ifndef _ZONECHANGER_H
#define _ZONECHANGER_H

#include "entity.h"

class ZoneChanger : public Entity
{
private:
	MAP_LIST destination;
	ZoneChanger* nextChanger;
	float destStartX;
	float destStartY;
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

	// When exiting the current map, get/set where to start the character on the next map
	float getDestinationStartX() { return destStartX; }
	float getDestinationStartY() { return destStartY; }
	
	void setDestinationStartingPos(int tileX, int tileY);

};



#endif