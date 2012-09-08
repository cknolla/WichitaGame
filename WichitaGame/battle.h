//battle.h
//Main code for battle initialization

#include "textureManager.h"
#include "entity.h"
#include "character.h"
#include "text.h"
#include "monster.h"
#include "bgfg.h"
#include "battleMenu.h"
#include <sstream>

#ifndef _BATTLE_H
#define _BATTLE_H


namespace battleNS
{
	const float PLAYER1_X = 100.0;
	const float PLAYER1_Y = 500.0;
	const float MONSTER1_X = GAME_WIDTH - 100.0;
	const float MONSTER1_Y = 500.0;
};

class Battle
{
private:
	bool active;
	TextureManager backgroundTexture;
	TextureManager foregroundTexture;
	TextureManager sceneTexture;
	TextureManager uiTexture;
	TextureManager playerTexture;
	TextureManager monsterTexture;
	Bgfg background;
	Bgfg foreground;
	Image scene;
	Image ui;
	Text playerHealthText;
	Text monsterHealthText;
	Character player;
	Monster* firstMonster;
	Monster testMonster;
	BattleMenu menu;
public:
	Battle();
	~Battle();

	//Initialization function prototype
	bool initialize(Game* gamePtr, const char* sceneTextureFile, const char* bgTextureFile = NULL, const char* fgTextureFile = NULL);

	void update(float frameTime);
	// render battle screen each frame
	void render();

	Bgfg* getForeground() { return &foreground; }

	Bgfg* getBackground() { return &background; }

	// return whether battle is open
	bool getActive() { return active; }
	// set whether battle is open
	void setActive(bool isActive) { active = isActive; }

	void onLostDevice();
	void onResetDevice();

	
};
// Template for converting items to strings
template<typename T>
std::string toString(T t)
{
	std::stringstream s;
	s << t;
	return s.str();
}

#endif