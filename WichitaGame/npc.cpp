
#include "npc.h"

NPC::NPC()
{
	nextNPC = NULL;
	// half second between animation frames
	frameDelay = 0.5f;
	// 5 seconds before pausing
	pauseInterval = 5.0f;
	// 2 second pause
	pauseTime = 2.0f;
	speaking = false;
	responses = 0;
	selectedResponse = 0;
	actualResponse = 0;
	messageNum = 0;
	dialogBox = NULL;
}

NPC::~NPC()
{}

bool NPC::initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM, Image* gameDialogBox)
{
	gameConfig = gamePtr->getGameConfig();
	// image must be initialized first in order to use sprite size data
	bool result = Entity::initialize(gamePtr, width, height, ncols, textureM);
	// active and set collision box area
	active = true;
	edge.left = -spriteData.width/2;
	edge.right = spriteData.width/2;
	edge.top = -spriteData.height/2;
	edge.bottom = spriteData.height/2;

	dialogBox = gameDialogBox;

	if(!dialogText.initialize(gamePtr->getGraphics(), SPRITE_TEXT_FILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing NPC dialogText"));
	dialogText.setFontHeight(14);

	if(!responseText.initialize(gamePtr->getGraphics(), SPRITE_TEXT_FILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing NPC response Text"));
	responseText.setFontHeight(14);

//	dialogText.setProportional(true);
	if(!selectorTexture.initialize(gamePtr->getGraphics(), "pictures/text/selector.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing NPC selector texture"));
	if(!selector.initialize(gamePtr->getGraphics(), 0, 0, 0, &selectorTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing NPC selector image"));

	return result;
}

void NPC::update(float frameTime)
{
	calcVelocity(frameTime);
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
	// update Image to animate
	if(velocity.x == 0.0 && velocity.y == 0.0) {
		// don't animate if not moving
		setLoop(false);
	} else {
		// animate
		setLoop(true);
		Entity::update(frameTime);
	}
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

void NPC::calcVelocity(float frameTime)
{
	float angle, xLength, yLength, tempX, tempY;
	static int xSign = 1;
	static int ySign = 1;
	xLength = moseyEndX-moseyStartX;
	yLength = moseyEndY-moseyStartY;

	static float timeTillPause = pauseInterval;
	static float timeTillMosey = pauseTime;

	if(speaking) {
		// prevent movement while speaking
		setVelocity(VECTOR2(0.0,0.0));
		return;
	}

	if(timeTillPause > 0.0)
		timeTillPause -= frameTime;
	else if(timeTillMosey > 0.0) {
		// NPC should be paused
		setVelocity(VECTOR2(0.0,0.0));
		timeTillMosey -= frameTime;
		return;
	}
	else {
		timeTillPause = pauseInterval;
		timeTillMosey = pauseTime;
	}
	// if he is not paused, continue on to mosey...
	// if vector is negative, it's moving left
	if(xLength < 0) {
		setFrames(2,3);
		flipHorizontal(true);
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
	
	// if vector is negative, he's moving up
	if(yLength < 0) {
		setFrames(4,5);
		// if the NPC is higher than that the endpoint, swap endpoint and startpoint
		if(spriteData.y < moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
			setCurrentFrame(0);
		}
	} else if(yLength > 0) {
		setFrames(0,1);
		if(spriteData.y > moseyEndY) {
			tempY = moseyStartY;
			moseyStartY = moseyEndY;
			moseyEndY = tempY;
			setCurrentFrame(4);

		}
	}
	if(xLength == 0.0) {
		if(yLength < 0)
			angle = -(float)PI/2; // prevent divide by 0 when xLength is 0. atan(inf) = -PI/2
		if(yLength > 0)
			angle = (float)PI/2;
	} else if(xLength < 0)
		angle = atan(yLength/xLength)+((float)PI); // rotate by 180 degrees
	else
		angle = atan(yLength/xLength);
	setVelocity(VECTOR2(npcNS::MOVE_SPEED*cos(angle)*xSign, npcNS::MOVE_SPEED*sin(angle)*ySign));
}

void NPC::speak()
{
	// This is a render function, so it must be within drawSprite
	// called from Map::render()
	int prevResponses = responses;
	if(speaking) {
		responses = 0;
		dialogBox->setX(gameConfig->getDialogBoxAnchorX());
		dialogBox->setY(gameConfig->getDialogBoxAnchorY());
		dialogBox->draw(graphicsNS::ALPHA75);
		if(messageNum == 0)
			sprintf_s(dialog, "O hai. I'm the owner of the dialog box. You should be able to fit 6 long lines of text in here, or you could do 4 lines of text and 2 selection options. Or, you could even do 3 lines of text and 3 selection options. Logic would tell you that you could also do 2 lines of text and 4 selection options. Understand that this dialog is long and useless for the sake of testing word-wrap functionality. Now leave me alone.");
		if(messageNum == 1) {
			sprintf_s(dialog, "Answer!");
			addResponses("Yes", "No");
		}
		if(messageNum == 2) {
			if(actualResponse == 0)
				sprintf_s(dialog, "You said yes");
			else if(actualResponse == 1)
				sprintf_s(dialog, "You said no");
		}
		dialogText.print(dialog, (int)dialogBox->getX() + textNS::BORDER_BUFFER, (int)dialogBox->getY() + textNS::BORDER_BUFFER, textNS::WORD_WRAP_LIMIT);
		if(prevResponses != responses) { // if number of responses was changed in this frame, set the selector to default to the top option
			selector.setX(dialogBox->getX() + textNS::BORDER_BUFFER);
			selector.setY(dialogBox->getY() + dialogBox->getHeight() - textNS::BORDER_BUFFER - responseText.getFontHeight()*responses);
			selectedResponse = 0;
		}
		if(responses)
			selector.draw();
	} 
}

void NPC::addResponses(const char* response1, const char* response2)
{
	responses = 2;
	responseText.print(response1, (int)dialogBox->getX() + textNS::BORDER_BUFFER + 16, (int)dialogBox->getY() + dialogBox->getHeight() - textNS::BORDER_BUFFER - responseText.getFontHeight()*2);
	responseText.print(response2, (int)dialogBox->getX() + textNS::BORDER_BUFFER + 16, (int)dialogBox->getY() + dialogBox->getHeight() - textNS::BORDER_BUFFER - responseText.getFontHeight());
}

void NPC::moveSelectorUp()
{
	// you can only move up if you're below response 0
	if(selectedResponse > 0) {
		selector.setY(selector.getY() - responseText.getFontHeight());
		selectedResponse--;
	}
}

void NPC::moveSelectorDown()
{
	// you can only move down if you're above the max response
	if(selectedResponse < responses-1) {
		selector.setY(selector.getY() + responseText.getFontHeight());
		selectedResponse++;
	}
}

void NPC::progressSpeech()
{
	if(responses)
		actualResponse = selectedResponse;
	messageNum++;
	if(messageNum > 2) {
		speaking = false; // if you surpass the max message count, stop speaking
		messageNum = 0; // start the dialog loop over
	}

}
