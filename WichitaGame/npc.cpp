
#include "npc.h"

NPC::NPC()
{
	nextNPC = NULL;
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
		xSign = -1;
		// if the NPC is further left than the endpoint, swap endpoint and startpoint
		if(spriteData.x < moseyEndX) {
			tempX = moseyStartX;
			moseyStartX = moseyEndX;
			moseyEndX = tempX;
		}
	} else { // if movement is positive, check for x > endpoint, then swap
		if(spriteData.x > moseyEndX) {
			tempX = moseyStartX;
			moseyStartX = moseyEndX;
			moseyEndX = tempX;
		}
	}
	
	if(yLength < 0) {
		ySign = -1;
		// if the NPC is higher than that the endpoint, swap endpoint and startpoint
		if(spriteData.y < moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
		}
	} else {
		if(spriteData.y > moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
		}
	}
	angle = atan(yLength/xLength);
	setVelocity(VECTOR2(npcNS::MOVE_SPEED*cos(angle)*xSign, npcNS::MOVE_SPEED*sin(angle)*ySign));
}