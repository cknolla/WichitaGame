

#include "battleMenu.h"

BattleMenu::BattleMenu()
{
	initialized = false;
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::initialize(Game* gamePtr)
{
	char errorStr[200];
	if (!selectorTexture.initialize(gamePtr->getGraphics(),"pictures/battle/battleSelector.png")) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector texture"));
	if(!selector.initialize(gamePtr->getGraphics(), 0, 0, 0, &selectorTexture)) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector image"));

	if (!texture.initialize(gamePtr->getGraphics(),textureFile)) {
		sprintf_s(errorStr, "Failed to initialize battle icon texture: %s", textureFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
	}

	initialized = true;
	return initialized;
}