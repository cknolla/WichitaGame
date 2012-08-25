
#include "bgfg.h"

Bgfg::Bgfg()
{
	parallax = 0.0;
	autoHscroll = 0.0;
	autoVscroll = 0.0;
}

Bgfg::~Bgfg()
{
}

void Bgfg::fillScreen()
{
	// save current location
	float prevX = spriteData.x;
	float prevY = spriteData.y;
	bool r0c0 = false, r0c1 = false, r0c2 = false, r1c0 = false, r1c1 = true, r1c2 = false, r2c0 = false, r2c1 = false, r2c2 = false;
	// r1c1 is always drawn. Up to 3 others can be drawn depending on position
	// r0c0 r0c1 r0c2
	// r1c0 r1c1 r1c2
	// r2c0 r2c1 r2c2

	// if the image is offscreen to the right, shift it a full width left
	if(spriteData.x > GAME_WIDTH)
		spriteData.x -= spriteData.width;
	// if offscreen to the left, shift it right
	if(spriteData.x + spriteData.width < 0)
		spriteData.x += spriteData.width;
	// if offscreen below, shift up
	if(spriteData.y > GAME_HEIGHT)
		spriteData.y -= spriteData.height;
	// if offscreen above, shift down
	if(spriteData.y + spriteData.height < 0)
		spriteData.y += spriteData.height;
	// now we know it's onscreen, so we'll draw it a max of 4 times to ensure the full screen is covered
	draw(); // draw it at its current location
	if(spriteData.x > 0) // if left edge is showing
		r1c0 = true;
	if(spriteData.x + spriteData.width < GAME_WIDTH) // if right edge is showing
		r1c2 = true;
	if(spriteData.y > 0) // if top edge is showing
		r0c1 = true;
	if(spriteData.y + spriteData.height < GAME_HEIGHT) // if bottom edge showing
		r2c1 = true;
	if(spriteData.x > 0 && spriteData.y > 0) // top left corner showing
		r0c0 = true;
	if(spriteData.y > 0 && spriteData.x + spriteData.width < GAME_WIDTH) // top right corner
		r0c2 = true;
	if(spriteData.x > 0 && spriteData.y + spriteData.height < GAME_HEIGHT) // bottom left corner
		r2c0 = true;
	if(spriteData.x + spriteData.width < GAME_WIDTH && spriteData.y + spriteData.height < GAME_HEIGHT) // bottom right corner
		r2c2 = true;

	if(r1c0) {
		spriteData.x -= spriteData.width;
		draw();
		spriteData.x = prevX;
	}
	if(r0c1) {
		spriteData.y -= spriteData.height;
		draw();
		spriteData.y = prevY;
	}
	if(r1c2) {
		spriteData.x += spriteData.width;
		draw();
		spriteData.x = prevX;
	}
	if(r2c1) {
		spriteData.y += spriteData.height;
		draw();
		spriteData.y = prevY;
	}
	if(r0c0) {
		spriteData.x -= spriteData.width;
		spriteData.y -= spriteData.height;
		draw();
		spriteData.x = prevX;
		spriteData.y = prevY;
	}
	if(r0c2) {
		spriteData.x += spriteData.width;
		spriteData.y -= spriteData.height;
		draw();
		spriteData.x = prevX;
		spriteData.y = prevY;
	}
	if(r2c0) {
		spriteData.x -= spriteData.width;
		spriteData.y += spriteData.height;
		draw();
		spriteData.x = prevX;
		spriteData.y = prevY;
	}
	if(r2c2) {
		spriteData.x += spriteData.width;
		spriteData.y += spriteData.height;
		draw();
		spriteData.x = prevX;
		spriteData.y = prevY;
	}
}
