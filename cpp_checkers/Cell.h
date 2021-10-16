#pragma once

enum CellStatusEnum
{
	EMPTY,
	PLAYER1,
	PLAYER2
};

class Cell
{
public:
	size_t i;
	size_t j;

	CellStatusEnum status;

	Cell();
};

