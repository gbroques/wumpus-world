#include "World.h"

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>

void displayHelp();

/**
 * Creates and "constructs" world
 */
World::World(string name)
{
	// to generate "truly" random numbers
	srand(time(0));

	//*****************************
	//  Initialize edge of board  *
	//*****************************

	// initialize top and bottom edge
	int row = 0;
	int col = 0;
	int numOfRows = 0;
	while (numOfRows < 2)
	{
		board[row][col].setName("edge");
		board[row][col].setType(EDGE);
		board[row][col].setFacing(NONE);
		board[row][col].setBreeze(false);
		board[row][col].setStench(false);

		col++;

		if (col == 7)
		{
			row = 6;
			col = 0;
			numOfRows++;
		}
	}

	// initialize left and right edge
	row = 1;
	col = 0;
	int numOfCols = 0;
	while (numOfCols < 2)
	{
		board[row][col].setName("edge");
		board[row][col].setType(EDGE);
		board[row][col].setFacing(NONE);
		board[row][col].setBreeze(false);
		board[row][col].setStench(false);

		row++;

		if (row == 6)
		{
			row = 1;
			col = 6;
			numOfCols++;
		}
	}

	//*********************
	//  Initialize board  *
	//*********************

	// initalize blank squares
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			board[i][j].setName("blank");
			board[i][j].setType(BLANK);
			board[i][j].setFacing(NONE);
			board[i][j].setBreeze(false);
			board[i][j].setStench(false);
		}
	}

	// initalize 3-8 pits
	int numOfPits = rand() % 6 + 3;
	int x, y;
	for (int i = 0; i < numOfPits; i++)
	{
		x = rand() % 5 + 1;
		y = rand() % 5 + 1;
		board[x][y].setName("Pit");
		board[x][y].setType(PIT);
		board[x][y].setFacing(NONE);
	}

	// get coordinates for wumpus and gold
	// that do not equal players starting position
	int x1, y1, x2, y2;
	do
	{
		do
		{
			x1 = rand() % 5 + 1;
			y1 = rand() % 5 + 1;
		}
		while (x1 == 5 && y1 == 1);

		do
		{
			x2 = rand() % 5 + 1;
			y2 = rand() % 5 + 1;
		}
		while (x2 == 5 && y2 == 1);
	}
	while (x1 == x2 && y1 == y2);

	// initalize wumpus
	board[x2][y2].setName("Wumpus");
	board[x2][y2].setType(WUMPUS);
	board[x2][y2].setFacing(SOUTH);

	// intialize gold
	board[x1][y1].setName("Gold");
	board[x1][y1].setType(GOLD);
	board[x1][y1].setFacing(NONE);

	// initialize breeze and stench
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			// check adjacent squares
			if (board[i+1][j].getType() == PIT ||
				board[i-1][j].getType() == PIT ||
				board[i][j+1].getType() == PIT ||
				board[i][j-1].getType() == PIT
			)
				board[i][j].setBreeze(true);

			if (board[i+1][j].getType() == WUMPUS ||
				board[i-1][j].getType() == WUMPUS ||
				board[i][j+1].getType() == WUMPUS ||
				board[i][j-1].getType() == WUMPUS
			)
				board[i][j].setStench(true);
		}
	}

	// intialize player
	board[5][1].setName(name);
	board[5][1].setType(PLAYER);
	board[5][1].setFacing(EAST);
}


/**
 * Prints out board
 */

void World::displayBoard()
{
	int numOfPipes = 6;
	int numOfCols = 5;
	int fieldWidth = 3;
	int boardWidth = fieldWidth * numOfCols + numOfPipes;

	cout << "\n";
	for (int i = 0; i < boardWidth; i++)
				cout << "-";
	cout << "\n";
	for (int i = 1; i <= 5; i++)
	{
		cout << "|";
		for (int j = 1; j <= 5; j++)
		{
			Type type = board[i][j].getType();
			bool isBreeze = board[i][j].getBreeze();
			bool isStench = board[i][j].getStench();

			string output;

			if (type == BLANK)
			{
				output += "";
			}
			else if (type == PLAYER)
			{
				Facing direction = board[i][j].getFacing();
				if (direction == NORTH)
					output += "^";
				else if (direction == EAST)
					output += ">";
				else if (direction == SOUTH)
					output += "v";
				else if (direction == WEST)
					output += "<";
			}
			else if (type == PIT)
			{
				output += "X";
			}
			else if (type == WUMPUS)
			{
				output += "W";
			}
			else if (type == GOLD)
			{
				output += "G";
			}

			if (isStench)
				output += "*";

			if (isBreeze)
				output += "~";

			cout << left << setw(fieldWidth) << output;
			cout << "|";
		}
		cout << "\n";
		for (int i = 0; i < boardWidth; i++)
			cout << "-";
		cout << "\n";
	}
	cout << "\n";
}


/**
 * Updates state of board based on move
 */

string World::updateBoard(string move)
{
	// convert move to lowercase
	for (int i = 0, n = move.length(); i < n; i++)
		move[i] = tolower(move[i]);

	// player coordinates
	int px, py;

	// find player
	for (int i = 1; i <= 5; i++)
	{
		for(int j = 1; j <= 5; j++)
		{
			if (board[i][j].getType() == PLAYER)
			{
				px = i;
				py = j;
			}
		}
	}

	Facing direction = board[px][py].getFacing();
	string typeOfMove;

	// move forward
	if (move == "w")
	{
		GameSquare* adjacentSquare;

		if (direction == NORTH)
		{
			adjacentSquare = &(board[px-1][py]);
			if (adjacentSquare->getType() == BLANK)
				board[px][py].swapSquares(*adjacentSquare);

			typeOfMove = adjacentSquare->getName();
		}
		else if (direction == EAST)
		{
			adjacentSquare = &(board[px][py+1]);
			if (adjacentSquare->getType() == BLANK)
				board[px][py].swapSquares(*adjacentSquare);

			typeOfMove = adjacentSquare->getName();
		}
		else if (direction == SOUTH)
		{
			adjacentSquare = &(board[px+1][py]);
			if (adjacentSquare->getType() == BLANK)
				board[px][py].swapSquares(*adjacentSquare);

			typeOfMove = adjacentSquare->getName();
		}
		else if (direction == WEST)
		{
			adjacentSquare = &(board[px][py-1]);
			if (adjacentSquare->getType() == BLANK)
				board[px][py].swapSquares(*adjacentSquare);

			typeOfMove = adjacentSquare->getName();
		}
	}

	// rotate clockwise
	else if (move == "d")
	{
		int direction = ((board[px][py].getFacing() + 1) % 5);
		if (board[px][py].getFacing() == WEST)
			direction++;

		board[px][py].setFacing(static_cast<Facing>(direction));
	}

	// rotate counter-clockwise
	else if (move == "a")
	{
		int direction = ((board[px][py].getFacing() - 1) % 5);
		if (board[px][py].getFacing() == NORTH)
			direction = WEST;

		board[px][py].setFacing(static_cast<Facing>(direction));
	}

	// shoot wumpus
	else if (move == "s")
	{
		GameSquare* adjacentSquare;

		if (direction == NORTH)
		{
			adjacentSquare = &(board[px-1][py]);
			if (adjacentSquare->getType() == WUMPUS)
			{
				adjacentSquare->setName("blank");
				adjacentSquare->setType(BLANK);
				adjacentSquare->setFacing(NONE);
			}
		}
		else if (direction == EAST)
		{
			adjacentSquare = &(board[px][py+1]);
			if (adjacentSquare->getType() == WUMPUS)
			{
				adjacentSquare->setName("blank");
				adjacentSquare->setType(BLANK);
				adjacentSquare->setFacing(NONE);
			}
		}
		else if (direction == SOUTH)
		{
			adjacentSquare = &(board[px+1][py]);
			if (adjacentSquare->getType() == WUMPUS)
			{
				adjacentSquare->setName("blank");
				adjacentSquare->setType(BLANK);
				adjacentSquare->setFacing(NONE);
			}
		}
		else if (direction == WEST)
		{
			adjacentSquare = &(board[px][py-1]);
			if (adjacentSquare->getType() == WUMPUS)
			{
				adjacentSquare->setName("blank");
				adjacentSquare->setType(BLANK);
				adjacentSquare->setFacing(NONE);
			}
		}
	}

	// help
	else if (move == "help")
	{
		displayHelp();
	}

	// quit game
	else if (move == "quit")
	{
		cout << "\nPlay again soon!\n";
		exit(0);
	}

	// invalid move
	else
	{
		cout << "\nInvalid move!\n";
	}

	return typeOfMove;
}


/**
 * Checks if win or lose
 */

void World::checkBoard(string typeOfMove)
{
	if (typeOfMove == "Wumpus" || typeOfMove == "Pit")
	{
		cout << "\nYou have died!\n";
		exit(0);
	}

	if (typeOfMove == "Gold")
	{
		cout << "\nYou win!\n";
		exit(0);
	}
}


/**
 * Prints out directions
 */

void displayHelp()
{
	cout << "\n";

	string directions[] = {
		"> = You",
		"X = Pit",
		"~ = Breeze",
		"W = Wumpus",
		"* = Stench",
		"w = Move forward",
		"d = Turn right",
		"a = Turn left",
		"s = Shoot"
	};

	int stars = 26;
	int padding = 5;
	int fieldWidth = 16;
	int numOfDirections = 9;

	for (int i = 0; i < stars; i++)
		cout << "*";
	cout << "\n";

	for (int i = 0; i < numOfDirections; i++)
	{
		cout << setw(padding) << left << "*";
		cout << setw(fieldWidth) << left << directions[i];
		cout << setw(padding) << right << "*";
		cout << endl;
	}

	for (int i = 0; i < stars; i++)
		cout << "*";
	cout << "\n";
}

