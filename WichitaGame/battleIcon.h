
#ifndef _BATTLEICON_H
#define _BATTLEICON_H

#include "game.h"
#include "image.h"

class BattleIcon : public Image
{
private:
	bool initialized;
	TextureManager texture;
public:
	BattleIcon();
	~BattleIcon();

	bool initialize(Game* gamePtr, const char* textureFile);

	void onLostDevice() { texture.onLostDevice(); }
	void onResetDevice() { texture.onResetDevice(); }

};



#endif