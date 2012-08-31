
#ifndef _BATTLEMENU_H
#define _BATTLEMENU_H

#include "battleIcon.h"

namespace battleMenuNS
{
	const int MAX_CHARACTERS = 3;
	const int MAX_CATEGORIES = 5;
	const int MAX_ACTIONS = 10;
	// anchor is the reference point for the first character icon to draw at
	const float ANCHOR_X = 100.0;
	const float ANCHOR_Y = 100.0;
	const float ICON_SPACING = 5.0; // pixels to space icons apart by
}

class BattleMenu
{
private:
	bool initialized;
	BattleIcon character[battleMenuNS::MAX_CHARACTERS];
	BattleIcon category[battleMenuNS::MAX_CATEGORIES];
	BattleIcon action[battleMenuNS::MAX_ACTIONS];
	TextureManager selectorTexture;
	Image selector;
	Input* input;
	GameConfig* config;
public:
	BattleMenu();
	~BattleMenu();

	bool initialize(Game* gamePtr);

	// check for input and move the selector
	void update(float frameTime);

	// draw the menu
	void render();
	

};



#endif