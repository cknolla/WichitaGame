
#include "battle.h"

Battle::Battle()
{
	firstMonster = NULL;
}

Battle::~Battle()
{
}

bool Battle::initialize(Game* gamePtr, const char* sceneTextureFile, const char* bgTextureFile, const char* fgTextureFile)
{
	if(bgTextureFile) {
		//Initialize the background texture
		if(!backgroundTexture.initialize(gamePtr->getGraphics(), bgTextureFile))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle background texture"));
		//Initialize the background image
		background.initialize(gamePtr->getGraphics(),0,0,0,&backgroundTexture);
	}
	if(fgTextureFile) {
		//Initialize the foreground texture
		if(!foregroundTexture.initialize(gamePtr->getGraphics(), fgTextureFile))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle foreground texture"));
		//Initialize the foreground image
		foreground.initialize(gamePtr->getGraphics(),0,0,0,&foregroundTexture);
	}


	//Initialize the scene texture
	if(!sceneTexture.initialize(gamePtr->getGraphics(), sceneTextureFile))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle scene texture"));
	//Initialize the scene image
	scene.initialize(gamePtr->getGraphics(),0,0,0,&sceneTexture);

	// initialize ui
	if(!uiTexture.initialize(gamePtr->getGraphics(), "pictures/battle/uiPlaceholder.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle ui texture"));
	//Initialize the ui image
	ui.initialize(gamePtr->getGraphics(),0,0,0,&uiTexture);
	ui.setX(0.0);
	ui.setY((float)(GAME_HEIGHT - ui.getHeight()));


	// initialize player
	if(!playerTexture.initialize(gamePtr->getGraphics(), "pictures/testcharacter01.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle player texture"));

	if(!player.initialize(gamePtr, 34, 34, 2, &playerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle player image"));
	player.setCurrentFrame(3);
	player.setX(battleNS::PLAYER1_X);
	player.setY(battleNS::PLAYER1_Y);

	// initialize monster
	if(!monsterTexture.initialize(gamePtr->getGraphics(),"pictures/testcharacter02.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle monster texture"));

	if(!testMonster.initialize(gamePtr, 34, 34, 2, &monsterTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing battle monster image"));
	testMonster.setCurrentFrame(3);
	testMonster.flipHorizontal(true);
	testMonster.setX(battleNS::MONSTER1_X - testMonster.getWidth());
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

	menu.initialize(gamePtr);

	return true;
}

void Battle::update(float frameTime)
{
	// autoscroll horizontally if not 0
	if(background.getAutoHscroll()) 
		background.setX( background.getX() + (frameTime * background.getAutoHscroll()));

	// autoscroll vertically if not 0
	if(background.getAutoVscroll())
		background.setY( background.getY() + (frameTime * background.getAutoVscroll()));

	// autoscroll horizontally if not 0
	if(foreground.getAutoHscroll()) 
		foreground.setX( foreground.getX() + (frameTime * foreground.getAutoHscroll()));

	// autoscroll vertically if not 0
	if(foreground.getAutoVscroll())
		foreground.setY( foreground.getY() + (frameTime * foreground.getAutoVscroll()));

	menu.update(frameTime);
}

void Battle::render()
{
	char buffer[200];
	// Draw background
	background.fillScreen();
	// Draw scene image
	scene.draw();
	

	// draw entities
	player.draw();
	testMonster.draw();
	
	// Draw foreground
	foreground.fillScreen();
	// draw UI boxes
	ui.draw(graphicsNS::ALPHA50);
	//Print player health
	sprintf_s(buffer, "Player HP: %.0f", player.getHealth());
	playerHealthText.print(buffer, 100, GAME_HEIGHT-100);

	sprintf_s(buffer, "Monster HP: %.0f", testMonster.getHealth());
	monsterHealthText.print(buffer, GAME_WIDTH-100, GAME_HEIGHT-100, textNS::RIGHT);

	menu.render();
}

void Battle::onLostDevice()
{
	sceneTexture.onLostDevice();
	playerHealthText.onLostDevice();
}

void Battle::onResetDevice()
{
	sceneTexture.onResetDevice();
	playerHealthText.onResetDevice();
}