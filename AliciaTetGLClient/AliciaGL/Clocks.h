#ifndef CLOCKS_H
#define CLOCKS_H
#include <iostream>


class Clock 
{
public:
	Clock();
	~Clock();
	float deltaTime;
	float lastFrameTime ;
	float INTERVAL ;
	float currentFrameTime ;
};

#endif