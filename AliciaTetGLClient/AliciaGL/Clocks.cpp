#include "Clocks.h"

Clock::Clock()
{
	deltaTime = 0.0f;
	lastFrameTime = 0.0f;
	INTERVAL = 1.0f;
	currentFrameTime = 0.0f;
}

Clock::~Clock() {
	std::cout << "Counting stopped..." << std::endl;
}