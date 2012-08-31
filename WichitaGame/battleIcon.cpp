

#include "battleIcon.h"

BattleIcon::BattleIcon()
{
	initialized = false;
}

BattleIcon::~BattleIcon()
{
}

bool BattleIcon::initialize(Game* gamePtr, const char* textureFile)
{
	char errorStr[200];
	if (!texture.initialize(gamePtr->getGraphics(),textureFile)) {
		sprintf_s(errorStr, "Failed to initialize battle icon texture: %s", textureFile);
		throw(GameError(gameErrorNS::FATAL_ERROR, errorStr));
	}
	if(!selector.initialize(gamePtr->getGraphics(), 0, 0, 0, &selectorTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector image"));

	initialized = true;
	return initialized;
}

