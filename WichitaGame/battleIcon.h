
#ifndef _BATTLEICON_H
#define _BATTLEICON_H

#include "game.h"
#include "image.h"

class BattleIcon : public Image
{
private:
	bool initialized;
	TextureManager texture;
	bool parent;
	COLOR_ARGB filter;
public:
	BattleIcon();
	~BattleIcon();

	bool initialize(Game* gamePtr, const char* textureFile);

	bool getInitialized() { return initialized; }

	// get whether this icon is a parent to a lower tier icon
	bool getParent() { return parent; }
	// set whether this icon is a parent to a lower tier icon
	void setParent(bool isParent);

	// return filter for drawing
	COLOR_ARGB getFilter() { return filter; }

	void onLostDevice() { texture.onLostDevice(); }
	void onResetDevice() { texture.onResetDevice(); }

};



#endif