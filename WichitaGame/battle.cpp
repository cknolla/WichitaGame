
#include "battle.h"

Battle::Battle()
{
	player = NULL;
}

Battle::~Battle()
{
}

bool Battle::initialize(Graphics *g,TextureManager *textureM, Entity* p1)
{
	//Setup the battle background with whatever background is passed in.
	battleBackground.initialize(g,0,0,0,textureM);
	
	//Initialize text with the font image
    if (!healthText.initialize(g,"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Set font size
	healthText.setFontHeight(12);
	//Turn off
	//healthText->setBold(false);

    healthText.setProportional(true);
    healthText.setFontColor(graphicsNS::WHITE); // solid

	// assign player pointer
	player = p1;

	return true;
}

void Battle::render()
{
	//Print Battle Background
	battleBackground.draw();
	//Print player health
	healthText.print(toString(player->getHealth()),100,GAME_HEIGHT-100);
}