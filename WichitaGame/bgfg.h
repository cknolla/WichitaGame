#ifndef _BGFG_H              // prevent multiple definitions if this 
#define _BGFG_H              // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"

// Bgfg = background foreground
class Bgfg : public Image
{
private:
	float parallax;
	float autoHscroll;
	float autoVscroll;
public:
	Bgfg();
	~Bgfg();

	// Get parallax scroll speed multiplier
	float getParallax() { return parallax; }

	// Set parallax scroll speed multiplier
	void setParallax(float p) { parallax = p; }

	// Get auto horizontal scroll speed multiplier
	float getAutoHscroll() { return autoHscroll; }

	// Set auto horizontal scroll speed multiplier
	void setAutoHscroll(float ahs) { autoHscroll = ahs; }

	// Get auto vertical scroll speed multiplier
	float getAutoVscroll() { return autoVscroll; }

	// Set auto vertical scroll speed multiplier
	void setAutoVscroll(float avs) { autoVscroll = avs; }

	// fill screen with background/foreground
	void fillScreen();
};

#endif
