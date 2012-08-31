
#ifndef _BATTLEMENU_H
#define _BATTLEMENU_H

#include "battleIcon.h"

namespace battleMenuNS
{
	const int MAX_CHARACTERS = 3;
	const int MAX_CATEGORIES = 5;
	const int MAX_ACTIONS = 10;
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
public:
	BattleMenu();
	~BattleMenu();

	bool initialize(Game* gamePtr);

	

};



#endif