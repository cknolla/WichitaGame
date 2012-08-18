// Zone changer

#ifndef _ZONECHANGER_H
#define _ZONECHANGER_H

#include "entity.h"

class ZoneChanger : public Entity
{
private:
	char destination[50];
	ZoneChanger* nextChanger;
public:
	ZoneChanger();
	~ZoneChanger();

	// only need destination for zoneChanger, but also have to initialize entity and image parents
	bool initialize(const char* dest, Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	const char* getDestination() { return destination; }

	// return next zone changer in linked list
	ZoneChanger* getNextChanger() { return nextChanger; }

	// set next zone changer in ZC list
	void setNextChanger(ZoneChanger* nz) { nextChanger = nz; }

};



#endif