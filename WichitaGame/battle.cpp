
#include "battle.h"

Battle::Battle()
{
	firstMonster = NULL;
}

Battle::~Battle()
{
}

bool Battle::initialize(Game* gamePtr, const char* textureFile)
{

	//Initialize the background texture
	if(!backgroundTexture.initialize(gamePtr->getGraphics(),textureFile))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle background texture"));
	//Setup the battle background with whatever background is passed in.
	battleBackground.initialize(gamePtr->getGraphics(),0,0,0,&backgroundTexture);

	if(!playerTexture.initialize(gamePtr->getGraphics(),"pictures/testcharacter01.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle player texture"));

	if(!player.initialize(gamePtr, 34, 34, 2, &playerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle player image"));
	player.setCurrentFrame(3);
	player.setX(battleNS::PLAYER1_X);
	player.setY(battleNS::PLAYER1_Y);

	if(!monsterTexture.initialize(gamePtr->getGraphics(),"pictures/testcharacter02.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle monster texture"));

	if(!testMonster.initialize(gamePtr, 34, 34, 2, &monsterTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle monster image"));
	testMonster.setCurrentFrame(3);
	testMonster.flipHorizontal(true);
	testMonster.setX(battleNS::MONSTER1_X);
	testMonster.setY(battleNS::MONSTER1_Y);
	
	//Initialize text with the font image
    if (!playerHealthText.initialize(gamePtr->getGraphics(),"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Initialize text with the font image
    if (!monsterHealthText.initialize(gamePtr->getGraphics(),"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Set font size
	playerHealthText.setFontHeight(12);
	monsterHealthText.setFontHeight(12);
	//Turn off
	//healthText->setBold(false);

//   playerHealthText.setProportional(true);
    playerHealthText.setFontColor(graphicsNS::WHITE); // solid
	monsterHealthText.setFontColor(graphicsNS::WHITE);

	return true;
}

void Battle::render()
{
	char buffer[200];
	//Print Battle Background
	battleBackground.draw();
	// draw UI boxes

	// draw entities
	player.draw();
	testMonster.draw();
	//Print player health
	sprintf_s(buffer, "Player HP: %.0f", player.getHealth());
	playerHealthText.print(buffer, 100, GAME_HEIGHT-100);

	sprintf_s(buffer, "Monster HP: %.0f", testMonster.getHealth());
	monsterHealthText.print(buffer, GAME_WIDTH-200, GAME_HEIGHT-100);
}

void Battle::onLostDevice()
{
	backgroundTexture.onLostDevice();
	playerHealthText.onLostDevice();
}

void Battle::onResetDevice()
{
	backgroundTexture.onResetDevice();
	playerHealthText.onResetDevice();
}