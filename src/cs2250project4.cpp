#include <iostream>
using namespace std;

#include "World.h"
#include "GameSquare.h"

int main() {
	// create world
	World world("Hero");

	// loop until game is won or quit or lost
	while (true)
	{
		// display board
		world.displayBoard();

		// prompt for move
		string move;
		cout << "Make a move (w, a, s, d, help, quit): ";
		getline(cin, move);

		// update board
		string typeOfMove = world.updateBoard(move);

		// check board
		world.checkBoard(typeOfMove);

	}

	return 0;
}
