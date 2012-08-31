// GameConfig class
#include "gameConfig.h"

GameConfig::GameConfig()
{
	// set defaults
	windowWidth = 1024;
	windowHeight = 768;
	gameWidth = 1024;
	gameHeight = 768;

	moveUpKey = 'W';
	moveDownKey = 'S';
	moveLeftKey = 'A';
	moveRightKey = 'D';
}

GameConfig::~GameConfig()
{
}

void GameConfig::initialize(HWND hw)
{
	hwnd = hw;
	// resize window to default settings in constructor
//	resizeWindow(windowWidth, windowHeight);
}

void GameConfig::resizeWindow(UINT newWindowWidth, UINT newWindowHeight)
{
	char errorStr[200];
	// complain if the new window size will cut off the game drawing area
	if(newWindowWidth < gameWidth) {
		sprintf_s(errorStr, "New window width (%d) passed to GameConfig::resizeWindow() is smaller than game width (%d)", newWindowWidth, gameWidth);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
	}
	if(newWindowHeight < gameHeight) {
		sprintf_s(errorStr, "New window height (%d) passed to GameConfig::resizeWindow() is smaller than game height (%d)", newWindowHeight, gameHeight);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
	}
	// if not, go ahead with resizing the window
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;
	if(!FULLSCREEN)             // if window
    {
        // Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);   // get size of client area of window
        MoveWindow(hwnd,
                   0,                                           // Left
                   0,                                           // Top
                   windowWidth+(windowWidth-clientRect.right),    // Right
                   windowHeight+(windowHeight-clientRect.bottom), // Bottom
                   TRUE);                                       // Repaint the window
    }

    // Show the window
//    ShowWindow(hwnd, nCmdShow);
}