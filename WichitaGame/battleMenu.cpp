

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
	int i;

	if (!selectorTexture.initialize(gamePtr->getGraphics(),"pictures/battle/battleSelector.png")) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector texture"));
	if(!selector.initialize(gamePtr->getGraphics(), 0, 0, 0, &selectorTexture)) 
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize battle selector image"));

	character[0].initialize(gamePtr, "pictures/battle/iconmagic.png");
	character[1].initialize(gamePtr, "pictures/battle/iconmagic.png");
	character[2].initialize(gamePtr, "pictures/battle/iconmagic.png");

	category[0].initialize(gamePtr, "pictures/battle/iconattack.png");
	category[1].initialize(gamePtr, "pictures/battle/iconattack.png");
	category[2].initialize(gamePtr, "pictures/battle/iconattack.png");
	category[3].initialize(gamePtr, "pictures/battle/iconattack.png");
	category[4].initialize(gamePtr, "pictures/battle/iconattack.png");

	action[0].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[1].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[2].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[3].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[4].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[5].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[6].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[7].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[8].initialize(gamePtr, "pictures/battle/iconitem.png");
	action[9].initialize(gamePtr, "pictures/battle/iconitem.png");
	
	for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++) {
		setColumn(character[i], 0);
		setRow(character[i], i);
	}
	for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++) {
		setColumn(category[i], 1);
		setRow(category[i], i);
	}
	for(i = 0; i < battleMenuNS::MAX_ACTIONS; i++) {
		setColumn(action[i], 2);
		setRow(action[i], i);
	}
		

	setColumn(selector, 0);
	setRow(selector, 0);

	initialized = true;
	return initialized;
}

void BattleMenu::update(float frameTime)
{
	BattleIcon* selectedIcon = getSelectedIcon();
	if(input->wasKeyPressed(config->getMoveRightKey())) {
		// if attempting to move right, ensure that the selector is no further than column 2
		if(getColumn(selector) < battleMenuNS::MAX_COLUMNS-1) {
			// if moving to category column, ensure a category is initialized, or if moving to action column, ensure an action is initialized
			if((getColumn(selector) == 0 && category[0].getInitialized()) || (getColumn(selector) == 1 && action[0].getInitialized())) {
				selectedIcon->setParent(true);
				setColumn(selector, getColumn(selector)+1);
				setRow(selector, 0);
			}
		}

	}
	if(input->wasKeyPressed(config->getMoveLeftKey())) {
		if(getColumn(selector) > 0) {
			// move the selector to the same row as its parent
			setRow(selector, getRow(*getParent(*selectedIcon)));
			// de-parent the previous column
			getParent(*selectedIcon)->setParent(false);
			// move the selector to the previous column
			setColumn(selector, getColumn(selector)-1);
			
		}
		
	}
	if(input->wasKeyPressed(config->getMoveDownKey())) {
		if(getColumn(selector) == 0) {
			// if the selector hasn't exceeded the maximum character count and the next character position is initialized...
			if(getRow(selector) < battleMenuNS::MAX_CHARACTERS-1 && character[getRow(selector)+1].getInitialized())
				// allowed to go down one row
				setRow(selector, getRow(selector)+1);
		}
		if(getColumn(selector) == 1) {
			if(getRow(selector) < battleMenuNS::MAX_CATEGORIES-1 && category[getRow(selector)+1].getInitialized())
				setRow(selector, getRow(selector)+1);
		}
		if(getColumn(selector) == 2) {
			if(getRow(selector) < battleMenuNS::MAX_ACTIONS-1 && action[getRow(selector)+1].getInitialized())
				setRow(selector, getRow(selector)+1);
		}
	}
	if(input->wasKeyPressed(config->getMoveUpKey())) {
		if(getRow(selector) > 0)
			setRow(selector, getRow(selector)-1);
	}
}

void BattleMenu::render()
{
	int i = 0;
	// always draw characters if initialized
	for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++) {
		if(character[i].getInitialized())
			character[i].draw(character[i].getFilter());
	}
	// only draw categories if in column 1 and they're initialized
	if(getColumn(selector) >= 1) {
		for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++) {
			if(category[i].getInitialized())
				category[i].draw(category[i].getFilter());
		}
	}
	// only draw actions if in column 2 and they're initialized
	if(getColumn(selector) >= 2) {
		for(i = 0; i < battleMenuNS::MAX_ACTIONS; i++) {
			if(action[i].getInitialized())
				action[i].draw(action[i].getFilter());
		}
	}

	selector.draw();
}

int BattleMenu::getColumn(Image &image)
{
	int i;
	for(i = 0; i < battleMenuNS::MAX_COLUMNS; i++) {
	if(image.getX() == battleMenuNS::ANCHOR_X + battleMenuNS::ICON_WIDTH*i + battleMenuNS::ICON_SPACING*i)
		return i;
	}
	// if nothing matches, there's an error, but return a number that will at least keep it on screen for debug purposes
	return battleMenuNS::MAX_COLUMNS+5;
}

void BattleMenu::setColumn(Image &image, int col)
{
	image.setX(battleMenuNS::ANCHOR_X + battleMenuNS::ICON_WIDTH*col + battleMenuNS::ICON_SPACING*col);
}

int BattleMenu::getRow(Image &image)
{
	int i;
	for(i = 0; i < battleMenuNS::MAX_ROWS; i++) {
	if(image.getY() == battleMenuNS::ANCHOR_Y + battleMenuNS::ICON_HEIGHT*i + battleMenuNS::ICON_SPACING*i)
		return i;
	}
	// if nothing matches, there's an error, but return a number that will at least keep it on screen for debug purposes
	return battleMenuNS::MAX_ROWS+5;
}

void BattleMenu::setRow(Image &image, int row)
{
	image.setY(battleMenuNS::ANCHOR_Y + battleMenuNS::ICON_HEIGHT*row + battleMenuNS::ICON_SPACING*row);
}

BattleIcon* BattleMenu::getSelectedIcon()
{
	int i;
	for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++) {
		if(selector.getX() == character[i].getX() && selector.getY() == character[i].getY())
			return &character[i];
	}
	for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++) {
		if(selector.getX() == category[i].getX() && selector.getY() == category[i].getY())
			return &category[i];
	}
	for(i = 0; i < battleMenuNS::MAX_ACTIONS; i++) {
		if(selector.getX() == action[i].getX() && selector.getY() == action[i].getY())
			return &action[i];
	}
	printf("Returning NULL from getSelectedIcon()");
	return NULL;
}

BattleIcon* BattleMenu::getParent(BattleIcon &icon)
{
	int i;
	int col = getColumn(icon);
	if(col == 1) { // parent is a character
		for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++) {
			if(character[i].getParent())
				return &character[i];
		}
	}
	if(col == 2) { // parent is a category
		for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++) {
			if(category[i].getParent())
				return &category[i];
		}
	}
	return NULL;
}

void BattleMenu::onLostDevice()
{
	int i;
	selectorTexture.onLostDevice();
	for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++)
		character[i].onLostDevice();
	for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++)
		category[i].onLostDevice();
	for(i = 0; i < battleMenuNS::MAX_ACTIONS; i++)
		action[i].onLostDevice();
}

void BattleMenu::onResetDevice()
{
	int i;
	selectorTexture.onResetDevice();
	for(i = 0; i < battleMenuNS::MAX_CHARACTERS; i++)
		character[i].onResetDevice();
	for(i = 0; i < battleMenuNS::MAX_CATEGORIES; i++)
		category[i].onResetDevice();
	for(i = 0; i < battleMenuNS::MAX_ACTIONS; i++)
		action[i].onResetDevice();
}