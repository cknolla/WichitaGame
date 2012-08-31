

#include "battleMenu.h"

BattleMenu::BattleMenu()
{
	input = NULL;
	config = NULL;
	initialized = false;
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::initialize(Game* gamePtr)
{
	input = gamePtr->getInput();
	config = gamePtr->getGameConfig();

	if (!selectorTexture.initialize(gamePtr->getGraphics(),"pictures/battle/battleSelector.png")) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector texture"));
	if(!selector.initialize(gamePtr->getGraphics(), 0, 0, 0, &selectorTexture)) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector image"));

	character[0].initialize(gamePtr, "pictures/battle/iconmagic.png");
	character[0].setX(battleMenuNS::ANCHOR_X);
	character[0].setY(battleMenuNS::ANCHOR_Y);

	category[0].initialize(gamePtr, "pictures/battle/iconattack.png");
	category[0].setX(character[0].getX() + character[0].getWidth() + battleMenuNS::ICON_SPACING);
	category[0].setY(battleMenuNS::ANCHOR_Y);

	selector.setX(character[0].getX());
	selector.setY(character[0].getY());

	initialized = true;
	return initialized;
}

void BattleMenu::update(float frameTime)
{
	if(input->wasKeyPressed(config->getMoveRightKey())) {
		// allow up to 3 columns
		if(selector.getX() < battleMenuNS::ANCHOR_X + selector.getWidth()*2 + battleMenuNS::ICON_SPACING*2)
			selector.setX(selector.getX() + selector.getWidth() + battleMenuNS::ICON_SPACING);
	}
	if(input->wasKeyPressed(config->getMoveLeftKey())) {
		if(selector.getX() > battleMenuNS::ANCHOR_X)
			selector.setX(selector.getX() - selector.getWidth() - battleMenuNS::ICON_SPACING);
	}
}

void BattleMenu::render()
{
	character[0].draw();

	if(selector.getX() > battleMenuNS::ANCHOR_X)
		category[0].draw();

	selector.draw();
}