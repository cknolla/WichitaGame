#ifndef _NPC_H                 
#define _NPC_H                
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include <math.h>

namespace npcNS
{
	const float MOVE_SPEED = 50.0f;
};

class NPC : public Entity
{
private:
	NPC* nextNPC;
	// start and stopping positions for mosey
public:
	NPC();
	virtual ~NPC();

	// progress the NPC mosey
	void update(float frameTime);

	// linked list functions
	NPC* getNextNPC() { return nextNPC; }
	void setNextNPC(NPC* nnpc) { nextNPC = nnpc; }

	void calcVelocity();

};

#endif