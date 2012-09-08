
#ifndef _BATTLEMENU_H
#define _BATTLEMENU_H

#include "battleIcon.h"

namespace battleMenuNS
{
	const int MAX_CHARACTERS = 3;
	const int MAX_CATEGORIES = 5;
	const int MAX_ACTIONS = 10;
	const int MAX_COLUMNS = 3;
	const int MAX_ROWS = 10;
	// anchor is the reference point for the first character icon to draw at
	const float ANCHOR_X = 200.0;
	const float ANCHOR_Y = 100.0;
	const int ICON_WIDTH = 32;
	const int ICON_HEIGHT = 32;
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

	// Get menu column position (0 based index)
	int getColumn(Image &image);
	// Set menu column position (0 based index)
	void setColumn(Image &image, int col);

	// Get menu row position (0 based index)
	int getRow(Image &image);
	// Set menu row position (0 based index)
	void setRow(Image &image, int row);

	// get a pointer to the battle icon currently selected by the selector
	BattleIcon* getSelectedIcon();

	// get the parent of the given icon
	BattleIcon* getParent(BattleIcon &icon);
public:
	BattleMenu();
	~BattleMenu();

	bool initialize(Game* gamePtr);

	// check for input and move the selector
	void update(float frameTime);

	// draw the menu
	void render();

	// handle textures when graphics device is lost/reset
	void onLostDevice();
	void onResetDevice();
	

};



#endif