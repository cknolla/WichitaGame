//battle.h
//Main code for battle initialization

#include "textureManager.h"
#include "entity.h"
#include "text.h"
#include <sstream>

#ifndef _BATTLE_H
#define _BATTLE_H

class Battle
{
private:
	Image battleBackground;
	Text healthText;
	Entity* player;
public:
	Battle();
	~Battle();

	//Initialization function prototype
	bool initialize(Graphics *g,TextureManager *textureM, Entity* p1);

	void update(float frameTime);
	// render battle screen each frame
	void render();

	//Retrieve the background image for drawing
	Image getBackground(){return battleBackground;};
	//void setHealth(Text sentHealth){health = sentHealth; return;};
	Text* getHealth(){return &healthText;};

	
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