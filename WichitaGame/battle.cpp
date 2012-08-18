
#include "battle.h"

Battle::Battle()
{
}
Battle::~Battle()
{
}

bool Battle::initialize(Graphics *g,TextureManager *textureM)
{
	battleBackground.initialize(g,0,0,0,textureM);
	return true;
}