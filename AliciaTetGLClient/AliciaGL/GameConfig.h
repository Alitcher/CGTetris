#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include "RenderConfig.h"

const float LeftPos = -0.5f;
const float TopPosY = 0.9f;
const float BotPosY = 0.4f;
const float gap = 0.2f;
const int shapesLength = 7;
bool inputKey[4];

const int BoardSize = 228;
const int ROW_COUNT = 12;
const int COL_COUNT = 18;
const int TET_GRID_COUNT = 16;

const int DOWN[2] = { 0, -1 };
const int LEFT[2] = { -1, 0 };
const int RIGHT[2] = { 1, 0 };

int RotateTet(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

#endif 