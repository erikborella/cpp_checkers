#include "Board.h"
#include <queue>

void Board::initializeCells()
{
	const uint32_t piecesPerPlayer = (boardSize * boardSize - 2 * boardSize) / 4;

	uint32_t counter = 0;

	for (size_t i = 0; i < boardSize && counter < piecesPerPlayer; i++)
	{
		for (size_t j = 0; j < boardSize && counter < piecesPerPlayer; j++)
		{
			if ((i + j) % 2 == 1) {
				counter++;
				cells[i][j].status = CellStatusEnum::PLAYER2;
			}
		}
	}

	counter = 0;

	for (int i = boardSize - 1; i >= 0 && counter < piecesPerPlayer; i--)
	{
		for (int j = boardSize - 1; j >= 0 && counter < piecesPerPlayer; j--)
		{
			if ((i + j) % 2 == 1) {
				counter++;
				cells[i][j].status = CellStatusEnum::PLAYER1;
			}
		}
	}

}

bool Board::isBoardPositionValid(int i, int j)
{
	return ((i >= 0 && i < boardSize) && (j >= 0 && j < boardSize));
}

void Board::convertToPosition(int posI, char posJ, size_t* i, size_t* j)
{
	*i = posI - 1;
	*j = posJ - 'A';
}


Board::Board(uint32_t boardSize)
{

	if (boardSize % 2 != 0)
		throw "boardSize cannot be an odd number";

	this->boardSize = boardSize;

	this->cells = new Cell*[boardSize];

	for (size_t i = 0; i < boardSize; i++)
	{
		cells[i] = new Cell[boardSize];
		for (size_t j = 0; j < boardSize; j++)
		{
			cells[i][j].i = i;
			cells[i][j].j = j;
		}
	}

	this->selectedCell = NULL;
	this->cachedValidPositions = {};

	cells[3][3].status = PLAYER2;
	cells[4][2].status = PLAYER1;
	cells[4][4].status = PLAYER1;
	cells[6][6].status = PLAYER1;
	cells[8][6].status = PLAYER1;


	//initializeCells();
}

Board::~Board()
{
	for (size_t i = 0; i < boardSize; i++)
		delete[] cells[i];
	
	delete[] cells;
}


bool Board::selectPiece(int posI, char posJ)
{
	size_t i;
	size_t j;

	convertToPosition(posI, posJ, &i, &j);

	std::vector<Position> validPositions = {};

	Cell& cell = cells[i][j];

	if (cell.status == EMPTY)
	{
		cachedValidPositions = validPositions;
		return false;
	}

	const CellStatusEnum opponent = (cell.status == PLAYER1) ? PLAYER2:PLAYER1;

	const size_t iDirection = (cell.status == PLAYER1) ? -1 : +1;
	const size_t jDirections[] = { 1, -1 };

	//find just emptys

	for (size_t jDirection : jDirections)
	{
		const size_t currentI = i + iDirection;
		const size_t currentJ = j + jDirection;
		
		if (isBoardPositionValid(currentI, currentJ))
		{
			Cell& currentCell = cells[currentI][currentJ];

			if (currentCell.status == EMPTY)
				validPositions.push_back(Position{ currentI, currentJ });
		}

	}

	std::queue<Position> metaMoves;
	metaMoves.push(Position{ i, j, {} });

	while (metaMoves.size() != 0)
	{
		Position p = metaMoves.front();
		metaMoves.pop();

		const size_t metaI = p.i; 
		const size_t metaJ = p.j;

		bool stopHere = true;

		for (size_t jDirection : jDirections)
		{
			const size_t currentI = metaI + iDirection;
			const size_t currentJ = metaJ + jDirection;

			if (isBoardPositionValid(currentI, currentJ))
			{
				Cell& currentCell = cells[currentI][currentJ];

				if (currentCell.status == opponent &&
					isBoardPositionValid(currentI + iDirection, currentJ + jDirection) &&
					cells[currentI + iDirection][currentJ + jDirection].status == EMPTY)
				{
					std::vector<Position> killeds = p.killeds;
					killeds.push_back(Position{ currentI, currentJ, {} });

					Position newMetaMove = { currentI + iDirection, currentJ + jDirection, killeds };

					metaMoves.push(newMetaMove);
					stopHere = false;
				}
			}
		}

		if (stopHere && (metaI != i || metaJ != j))
		{
			validPositions.push_back(Position{ metaI, metaJ, p.killeds });
		}

	}
	

	/*for (size_t jDirection : jDirections)
	{
		const size_t currentI = i + iDirection;
		const size_t currentJ = j + jDirection;

		if (isBoardPositionValid(currentI, currentJ))
		{
			Cell& currentCell = cells[currentI][currentJ];

			if (currentCell.status == EMPTY)
				validPositions.push_back(Position{ currentI, currentJ });

			else if (currentCell.status == opponent &&
				isBoardPositionValid(currentI + iDirection, currentJ + jDirection) &&
				cells[currentI + iDirection][currentJ + jDirection].status == EMPTY)
			{
				std::vector<Position> killeds{ Position{ currentI, currentJ } };
				validPositions.push_back(
					Position
					{ 
						currentI + iDirection, 
						currentJ + jDirection, 
						killeds 
					});
			}
		}
	}*/

	selectedCell = &cell;
	cachedValidPositions = validPositions;

	return true;
}

void Board::play(int posI, char posJ)
{
	if (selectedCell == NULL)
		return;

	size_t i;
	size_t j;

	convertToPosition(posI, posJ, &i, &j);

	for (Position p : cachedValidPositions) 
	{
		if (p.i == i && p.j == j) {
			cells[p.i][p.j].status = selectedCell->status;
			selectedCell->status = EMPTY;

			for (Position killedPieces : p.killeds)
				cells[killedPieces.i][killedPieces.j].status = EMPTY;
		}
	}

	cachedValidPositions = {};
	selectedCell = NULL;
}

void Board::printBoard()
{
	std::string buffer;

	const size_t maxPadding = std::to_string(boardSize).length();

	for (size_t i = 0; i <= maxPadding; i++)
	{
		buffer.append(" ");
	}

	for (char i = 'A'; i < 'A' + boardSize; i++)
	{
		buffer.push_back(i);
	}

	buffer.append("\n");

	for (size_t i = 0; i < boardSize; i++)
	{
		const int padding = maxPadding - std::to_string(i + 1).length();

		for (size_t p = 0; p < padding; p++)
			buffer.append(" ");

		buffer.append(std::to_string(i + 1)).append(" ");


		for (size_t j = 0; j < boardSize; j++)
		{
			Cell& currentCell = cells[i][j];

			std::string bkgColor = ((i + j) % 2 == 0) ? "\033[47m" : "\033[40m";

			if (cachedValidPositions.size() != 0) {
				for (Position p : cachedValidPositions) 
				{
					if (p.i == i && p.j == j)
						bkgColor = "\033[42m";
				}
			}

			buffer.append(bkgColor);

			if (currentCell.status == EMPTY)
				buffer.append(" ");
			else if (currentCell.status == PLAYER1)
				buffer.append("\033[31m").append("1").append("\033[0m");
			else
				buffer.append("\033[34m").append("2").append("\033[0m");

			buffer.append("\033[0m");
		}

		buffer.append("\n");
	}

	for (size_t i = 0; i <= maxPadding; i++)
	{
		buffer.append(" ");
	}

	for (char i = 'A'; i < 'A' + boardSize; i++)
	{
		buffer.push_back(i);
	}

	std::cout << buffer;
}
