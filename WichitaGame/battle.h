//battle.h
//Main code for battle initialization

#include "textureManager.h"
#include "image.h"
#include "text.h"
#include <sstream>

#ifndef _BATTLE_H
#define _BATTLE_H

class Battle
{
private:
	Image battleBackground;
	Text *healthText;
public:
	Battle();
	~Battle();

	//Initialization function prototype
	bool initialize(Graphics *g,TextureManager *textureM);
	//Retrieve the background image for drawing
	Image getBackground(){return battleBackground;};
	//void setHealth(Text sentHealth){health = sentHealth; return;};
	Text* getHealth(){return healthText;};
	
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