
#include "npc.h"

NPC::NPC()
{
	nextNPC = NULL;
	frameDelay = 0.5f;
}

NPC::~NPC()
{}

void NPC::update(float frameTime)
{
	calcVelocity();
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
	// update Image to animate
	Entity::update(frameTime);
}

void NPC::setMoseyStartingPos(int tileX, int tileY)
{
	moseyStartX = (float)tileX*TILE_WIDTH;
	moseyStartY = (float)tileY*TILE_HEIGHT;
//	spriteData.startX = (float)tileX*TILE_WIDTH;
//	spriteData.startY = (float)tileY*TILE_HEIGHT;
}

void NPC::setMoseyEndingPos(int tileX, int tileY)
{
	moseyEndX = (float)tileX*TILE_WIDTH;
	moseyEndY = (float)tileY*TILE_HEIGHT;    
}

void NPC::calcVelocity()
{
	float angle, xLength, yLength, tempX, tempY;
	int xSign = 1;
	int ySign = 1;
	xLength = moseyEndX-moseyStartX;
	yLength = moseyEndY-moseyStartY;
	// if vector is negative, it's moving left
	if(xLength < 0) {
		setFrames(2,3);
		flipHorizontal(true);
		xSign = -1;
		// if the NPC is further left than the endpoint, swap endpoint and startpoint
		if(spriteData.x < moseyEndX) {
			tempX = moseyStartX;
			moseyStartX = moseyEndX;
			moseyEndX = tempX;
		}
	} else if(xLength > 0) { // if movement is positive, check for x > endpoint, then swap
		setFrames(2,3);
		flipHorizontal(false);
		if(spriteData.x > moseyEndX) {
			tempX = moseyStartX;
			moseyStartX = moseyEndX;
			moseyEndX = tempX;
		}
	}
	
	if(yLength < 0) {
		ySign = -1;
		setFrames(4,5);
		// if the NPC is higher than that the endpoint, swap endpoint and startpoint
		if(spriteData.y < moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
		}
	} else if(yLength > 0) {
		setFrames(0,1);
		if(spriteData.y > moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
		}
	}
	angle = atan(yLength/xLength);
	setVelocity(VECTOR2(npcNS::MOVE_SPEED*cos(angle)*xSign, npcNS::MOVE_SPEED*sin(angle)*ySign));
}