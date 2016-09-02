#ifndef WORLD_H
#define WORLD_H

#include "GameSquare.h"

class World
{
	private:
		// 7x7 for 5x5 game with edge
		GameSquare board[7][7];

	public:
		World(string);

		void displayBoard();

		string updateBoard(string move);

		void checkBoard(string typeOfMove);
};

#endif
