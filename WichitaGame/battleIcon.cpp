

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
	Image::initialize(gamePtr->getGraphics(), 0, 0, 0, &texture);

	initialized = true;
	return initialized;
}

