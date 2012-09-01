

#include "battleIcon.h"

BattleIcon::BattleIcon()
{
	initialized = false;
	parent = false;
	filter = graphicsNS::WHITE;
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

void BattleIcon::setParent(bool isParent)
{
	parent = isParent;
	if(parent)
		filter = graphicsNS::YELLOW;
	else
		filter = graphicsNS::WHITE;
}

