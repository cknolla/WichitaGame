
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
	Image::update(frameTime);
}

void NPC::calcVelocity()
{
	float angle, xLength, yLength, tempX, tempY;
	int xSign = 1;
	int ySign = 1;
	xLength = spriteData.endX-spriteData.startX;
	yLength = spriteData.endY-spriteData.startY;
	// if vector is negative, it's moving left
	if(xLength < 0) {
		xSign = -1;
		// if the NPC is further left that the endpoint, swap endpoint and startpoint
		if(spriteData.x < spriteData.endX) {
			tempX = spriteData.startX;
			spriteData.startX = spriteData.endX;
			spriteData.endX = tempX;
			printf("startX = %.0f, startY = %.0f, spriteData.endX = %.0f, spriteData.endY = %.0f\n", spriteData.startX, spriteData.startY, spriteData.endX, spriteData.endY);
		}
	} else { // if movement is positive, check for x > endpoint, then swap
		if(spriteData.x > spriteData.endX) {
			tempX = spriteData.startX;
			spriteData.startX = spriteData.endX;
			spriteData.endX = tempX;
			printf("startX = %.0f, startY = %.0f, spriteData.endX = %.0f, spriteData.endY = %.0f\n", spriteData.startX, spriteData.startY, spriteData.endX, spriteData.endY);
		}
	}
	
	if(yLength < 0) {
		ySign = -1;
		// if the NPC is further left that the endpoint, swap endpoint and startpoint
		if(spriteData.y < spriteData.endY) {
			tempY = spriteData.startY;
			spriteData.startY = spriteData.endY;
			spriteData.endY = tempY;
			printf("startX = %.0f, startY = %.0f, spriteData.endX = %.0f, spriteData.endY = %.0f\n", spriteData.startX, spriteData.startY, spriteData.endX, spriteData.endY);
		}
	} else {
		if(spriteData.y > spriteData.endY) {
			tempY = spriteData.startY;
			spriteData.startY = spriteData.endY;
			spriteData.endY = tempY;
			printf("startX = %.0f, startY = %.0f, spriteData.endX = %.0f, spriteData.endY = %.0f\n", spriteData.startX, spriteData.startY, spriteData.endX, spriteData.endY);
		}
	}
	angle = atan(yLength/xLength);
	setVelocity(VECTOR2(npcNS::MOVE_SPEED*cos(angle)*xSign, npcNS::MOVE_SPEED*sin(angle)*ySign));
}