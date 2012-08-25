//battle.h
//Main code for battle initialization

#include "textureManager.h"
#include "entity.h"
#include "character.h"
#include "text.h"
#include "monster.h"
#include <sstream>

#ifndef _BATTLE_H
#define _BATTLE_H

class Battle
{
private:
	TextureManager backgroundTexture;
	Image battleBackground;
	Text playerHealthText;
	Text monsterHealthText;
	Character* player;
	Monster* firstMonster;
	Monster testMonster;
public:
	Battle();
	~Battle();

	//Initialization function prototype
	bool initialize(Graphics *g, const char* textureFile, Character* p1);

	void update(float frameTime);
	// render battle screen each frame
	void render();

	//Retrieve the background image for drawing
	Image getBackground(){return battleBackground;};

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