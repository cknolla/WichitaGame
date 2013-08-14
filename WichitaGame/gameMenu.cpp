

#include "gameMenu.h"

GameMenu::GameMenu()
{
	input = NULL;
	config = NULL;
	initialized = false;
	active = false;
}

GameMenu::~GameMenu()
{
}

bool GameMenu::initialize(Game* gamePtr)
{
	input = gamePtr->getInput();
	config = gamePtr->getGameConfig();

	if(!bgTexture.initialize(gamePtr->getGraphics(), "pictures/menuBg.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu background texture"));

	if(!bgImage.initialize(gamePtr->getGraphics(), 0, 0, 0, &bgTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu background image"));

	bgImage.setX(0.0);
	bgImage.setY(0.0);	

	initialized = true;
	return initialized;
}

void GameMenu::update(float frameTime)
{
	if(input->wasKeyPressed(config->getMenuKey())) {
		// reset menu here
		active = false; // turn off menu
	}
}

void GameMenu::render()
{
	bgImage.draw(graphicsNS::ALPHA85);
}

void GameMenu::onLostDevice()
{
	bgTexture.onLostDevice();
}

void GameMenu::onResetDevice()
{
	bgTexture.onResetDevice();
}
