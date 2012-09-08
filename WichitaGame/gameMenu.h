
#ifndef _GAMEMENU_H
#define _GAMEMENU_H

#include "game.h"
#include "image.h"

class GameMenu
{
private:
	bool initialized;
	bool active;
	
	Input* input;
	GameConfig* config;
	TextureManager bgTexture;
	Image bgImage;


public:
	GameMenu();
	~GameMenu();

	bool initialize(Game* gamePtr);

	// check for input and move the selector
	void update(float frameTime);

	// draw the menu
	void render();

	// return whether menu is open
	bool getActive() { return active; }
	// set whether menu is open
	void setActive(bool isActive) { active = isActive; }

	// handle textures when graphics device lost/reset
	void onLostDevice();
	void onResetDevice();

	

};



#endif