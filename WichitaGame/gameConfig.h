// GameConfig class

#ifndef _GAMECONFIG_H
#define _GAMECONFIG_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "gameError.h"
#include "constants.h"

class GameConfig
{
private:
	// a handle to the window
	HWND hwnd;
	UINT windowWidth, gameWidth;
	UINT windowHeight, gameHeight;
	UCHAR moveUpKey;
	UCHAR moveDownKey;
	UCHAR moveRightKey;
	UCHAR moveLeftKey;
	UCHAR actionKey;
public:
	GameConfig();
	~GameConfig();

	// initialize object
	void initialize(HWND hw);

	// get actual window width
	UINT getWindowWidth() { return windowWidth; }

	// get actual window height
	UINT getWindowHeight() { return windowHeight; }
	// to set window width/height, use resizeWindow(). This will avoid having mismatching variables to actual settings

	// resize window and game area using new values
	void resizeWindow(UINT newWindowWidth, UINT newWindowHeight);

	// get game drawing area width
	UINT getGameWidth() { return gameWidth; }
	// set game drawing area width
	void setGameWidth(UINT gw) { gameWidth = gw; }

	// get game drawing area height
	UINT getGameHeight() { return gameHeight; }
	// set game drawing area height
	void setGameHeight(UINT gh) { gameHeight = gh; }

	UCHAR getMoveUpKey() { return moveUpKey; }
	UCHAR getMoveDownKey() { return moveDownKey; }
	UCHAR getMoveRightKey() { return moveRightKey; }
	UCHAR getMoveLeftKey() { return moveLeftKey; }
	UCHAR getActionKey() { return actionKey; }

	void setMoveUpKey(UCHAR key) { moveUpKey = key; }
	void setMoveDownKey(UCHAR key) { moveDownKey = key; }
	void setMoveRightKey(UCHAR key) { moveRightKey = key; }
	void setMoveLeftKey(UCHAR key) { moveLeftKey = key; }
	void setActionKey(UCHAR key) { actionKey = key; }

};


#endif