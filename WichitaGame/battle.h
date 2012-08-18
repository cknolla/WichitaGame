//battle.h
//Main code for battle initialization

#include "textureManager.h"
#include "image.h"

#ifndef _BATTLE_H
#define _BATTLE_H

class Battle
{
private:
	Image battleBackground;
	bool battleOn;
public:
	Battle();
	~Battle();

	//Initialization function prototype
	bool initialize(Graphics *g,TextureManager *textureM);
	//Retrieve the background image for drawing
	Image getBackground(){return battleBackground;};
	bool setBattleOn(){return (battleOn=true);};
	bool getBattleOn(){return battleOn;};
	
};

#endif