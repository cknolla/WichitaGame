
#include "battle.h"

Battle::Battle()
{
	player = NULL;
	firstMonster = NULL;
}

Battle::~Battle()
{
}

bool Battle::initialize(Graphics *g, const char* textureFile, Character* p1)
{

	//Initialize the background texture
	if(!backgroundTexture.initialize(g,textureFile))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));
	//Setup the battle background with whatever background is passed in.
	battleBackground.initialize(g,0,0,0,&backgroundTexture);
	
	//Initialize text with the font image
    if (!playerHealthText.initialize(g,"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Initialize text with the font image
    if (!monsterHealthText.initialize(g,"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Set font size
	playerHealthText.setFontHeight(12);
	monsterHealthText.setFontHeight(12);
	//Turn off
	//healthText->setBold(false);

//   playerHealthText.setProportional(true);
    playerHealthText.setFontColor(graphicsNS::WHITE); // solid
	monsterHealthText.setFontColor(graphicsNS::WHITE);

	// assign player pointer
	player = p1;

	return true;
}

void Battle::render()
{
	char buffer[200];
	//Print Battle Background
	battleBackground.draw();
	//Print player health
	sprintf_s(buffer, "Player HP: %.0f", player->getHealth());
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