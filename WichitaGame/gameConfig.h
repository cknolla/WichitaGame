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
	UINT moveUpKey;
	UINT moveDownKey;
	UINT moveRightKey;
	UINT moveLeftKey;
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

	// get game drawing area width
	UINT getGameWidth() { return gameWidth; }
	// set game drawing area width
	void setGameWidth(UINT gw) { gameWidth = gw; }

	// get game drawing area height
	UINT getGameHeight() { return gameHeight; }
	// set game drawing area height
	void setGameHeight(UINT gh) { gameHeight = gh; }

	// resize window and game area using new values
	void resizeWindow(UINT newWindowWidth, UINT newWindowHeight);

};


#endif