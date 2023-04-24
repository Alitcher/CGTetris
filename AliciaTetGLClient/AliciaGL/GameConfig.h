#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include "RenderConfig.h"

const float LeftPos = -0.5f;
const float TopPosY = 0.9f;
const float BotPosY = 0.4f;
const float gap = 0.2f;
const int shapesLength = 7;
bool inputKey[4];

const int LowerBound = 1;

const int BoardSize = 228;
const int ROW_COUNT = 18;
const int COL_COUNT = 12;
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

std::wstring shapes[shapesLength];
std::wstring board;

int boardBit[BoardSize];
int boardBitTmp[BoardSize];
int tetrominoBitGrid[TET_GRID_COUNT];
int scoreCount = 20;
int stepsCount;
int randomTetromino;
int currentTetRotation = 0;
bool isGameover;
void printBoardGlobe() {
	system("cls");

	std::cout << "Score: " << scoreCount << std::endl;
	for (int i = 0; i < BoardSize; i++) {
		int row = i / COL_COUNT; // calculate row
		int col = i % COL_COUNT; // calculate column
		std::cout << boardBitTmp[i];
		if (col == COL_COUNT - 1) {
			std::cout << std::endl;
		}
	}
}

void printTetrominoBit()
{
	std::cout << std::endl;
	std::cout << "Current tetromino: " << std::endl;
	for (int i = 0; i < TET_GRID_COUNT; i++)
	{
		int row = i / 4; // calculate row
		int col = i % 4; // calculate column

		std::cout << tetrominoBitGrid[i];
		if (col == 4 - 1) {
			std::cout << std::endl;
		}
	}
}

void setBoard()
{

	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"XXXXXXXXXXXX");
}

void updateBitBoard()
{
	int row, col;
	for (int i = 0; i < BoardSize; ++i)
	{
		row = i / COL_COUNT; // calculate row
		col = i % COL_COUNT; // calculate column

		if (board.at(i) == L'X') {

			boardBit[i] = 1;
			boardBitTmp[i] = 1;
		}
	}
}

void setupTetrominos() {
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");

	shapes[1].append(L"..X.");
	shapes[1].append(L".XX.");
	shapes[1].append(L"..X.");
	shapes[1].append(L"....");

	shapes[2].append(L"....");
	shapes[2].append(L".XX.");
	shapes[2].append(L".XX.");
	shapes[2].append(L"....");

	shapes[3].append(L"..X.");
	shapes[3].append(L".XX.");
	shapes[3].append(L".X..");
	shapes[3].append(L"....");

	shapes[4].append(L".X..");
	shapes[4].append(L".XX.");
	shapes[4].append(L"..X.");
	shapes[4].append(L"....");

	shapes[5].append(L".X..");
	shapes[5].append(L".X..");
	shapes[5].append(L".XX.");
	shapes[5].append(L"....");

	shapes[6].append(L"..X.");
	shapes[6].append(L"..X.");
	shapes[6].append(L".XX.");
	shapes[6].append(L"....");


}
#endif 