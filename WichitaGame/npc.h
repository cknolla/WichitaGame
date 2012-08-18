#ifndef _NPC_H                 
#define _NPC_H                
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

class NPC : public Entity
{
private:
	NPC* nextNPC;
public:
	NPC();
	virtual ~NPC();

	// linked list functions
	NPC* getNextNPC() { return nextNPC; }
	void setNextNPC(NPC* nnpc) { nextNPC = nnpc; }
};

#endif