#ifndef _NPC_H                 
#define _NPC_H                
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include <math.h>

namespace npcNS
{
	const float MOVE_SPEED = 80.0f;
};

class NPC : public Entity
{
private:
	NPC* nextNPC;
	// start and stopping positions for mosey
	float moseyStartX, moseyStartY;
	float moseyEndX, moseyEndY;

public:
	NPC();
	virtual ~NPC();

	// progress the NPC mosey
	void update(float frameTime);

	// get/set functions for mosey
	float getMoseyStartX() { return moseyStartX; }
	float getMoseyStartY() { return moseyStartY; }
	void setMoseyStartX(float x) { moseyStartX = x; }
	void setMoseyStartY(float y) { moseyStartY = y; }
	// convert starting tile to true coordinates. set mosey start and sprite start position
	void setMoseyStartingPos(int tileX, int tileY);

	float getMoseyEndX() { return moseyEndX; }
	float getMoseyEndY() { return moseyEndY; }
	void setMoseyEndX(float x) { moseyEndX = x; }
	void setMoseyEndY(float y) { moseyEndY = y; }
	void setMoseyEndingPos(int tileX, int tileY);

	// linked list functions
	NPC* getNextNPC() { return nextNPC; }
	void setNextNPC(NPC* nnpc) { nextNPC = nnpc; }

	void calcVelocity();

};

#endif