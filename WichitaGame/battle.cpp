
#include "battle.h"

Battle::Battle()
{
}
Battle::~Battle()
{
}

bool Battle::initialize(Graphics *g,TextureManager *textureM)
{
	//initialize text instance
	healthText = new Text();

	//Setup the battle background with whatever background is passed in.
	battleBackground.initialize(g,0,0,0,textureM);
	
	//Initialize text with the font image
    if (!healthText->initialize(g,"pictures\\text\\CKfont.png"))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing font"));

	//Set font size
	healthText->setFontHeight(48);
	//Turn off
	//healthText->setBold(false);

    healthText->setProportional(true);
    healthText->setFontColor(graphicsNS::WHITE); // solid


	return true;
}