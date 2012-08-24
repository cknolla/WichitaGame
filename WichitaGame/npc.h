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
	// time between pauses
	float pauseInterval;
	// how long he stands still
	float pauseTime;

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
	// This gets set in Map::reset() - normally it's not needed, but it's available
	void setMoseyStartingPos(int tileX, int tileY);

	float getMoseyEndX() { return moseyEndX; }
	float getMoseyEndY() { return moseyEndY; }
	void setMoseyEndX(float x) { moseyEndX = x; }
	void setMoseyEndY(float y) { moseyEndY = y; }
	void setMoseyEndingPos(int tileX, int tileY);

	// time between pauses
	float getPauseInterval() { return pauseInterval; }
	void setPauseInterval(float pi) { pauseInterval = pi; }

	// amount of time he stands still
	float getPauseTime() { return pauseTime; }
	void setPauseTime(float pt) { pauseTime = pt; }

	// linked list functions
	NPC* getNextNPC() { return nextNPC; }
	void setNextNPC(NPC* nnpc) { nextNPC = nnpc; }

	void calcVelocity(float frameTime);

};

#endif