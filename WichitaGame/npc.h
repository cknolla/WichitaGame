#ifndef _NPC_H                 
#define _NPC_H                
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "text.h"
#include <math.h>

namespace npcNS
{
	const float MOVE_SPEED = 80.0f;
};

class NPC : public Entity
{
private:
	GameConfig* gameConfig;
	NPC* nextNPC;
	// start and stopping positions for mosey
	float moseyStartX, moseyStartY;
	float moseyEndX, moseyEndY;
	// time between pauses
	float pauseInterval;
	// how long he stands still
	float pauseTime;
	// NPC is currently speaking
	bool speaking;
	// Number of responses available to player in a selection dialog
	int responses;
	// number of currently selected response
	int selectedResponse;
	// response chosen in previous message
	int actualResponse;
	// ID number of message in database
	int messageNum;
	// char buffer to hold NPC dialog
	char dialog[3000];
	// sprite text to hold dialog
	Text dialogText;
	// sprite text to hold response choices
	Text responseText;
	// pointer to the game's dialog box
	Image* dialogBox;
	// selector texture
	TextureManager selectorTexture;
	// selector image
	Image selector;

	// add choices to a selection box
	void addResponses(const char* response1, const char* response2);
	void addResponses(const char* response1, const char* response2, const char* response3);

public:
	NPC();
	virtual ~NPC();

	// initialize NPC
	bool initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM, Image* gameDialogBox);

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

	// return whether this NPC is currently speaking
	bool getSpeaking() { return speaking; }
	// set whether this NPC should be speaking
	void setSpeaking(bool isSpeaking) { speaking = isSpeaking; }

	// select dialog and print to screen. Must be between spriteBegin/spriteEnd
	void speak();
	// progress to the next message and process response if applicable
	void progressSpeech();

	// move selector icon to the next option up
	void moveSelectorUp();

	// move selector icon to the next option down
	void moveSelectorDown();


};

#endif