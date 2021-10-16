#pragma once
#include "Cell.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

struct Position
{
	size_t i;
	size_t j;

	std::vector<Position> killeds;
};


class Board
{
private:
	uint32_t boardSize;
	Cell** cells;

	Cell* selectedCell;
	std::vector<Position> cachedValidPositions;

	void initializeCells();

	bool isBoardPositionValid(int i, int j);
	void convertToPosition(int posI, char posJ, size_t* i, size_t* j);

public:
	Board(uint32_t boardSize);
	~Board();

	bool selectPiece(int posI, char posJ);
	void play(int posI, char posJ);

	void printBoard();
};

