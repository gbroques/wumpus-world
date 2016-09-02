#ifndef GAMESQUARE_H
#define GAMESQUARE_H

#include <iostream>
using namespace std;

enum Type {EDGE, BLANK, PLAYER, PIT, WUMPUS, GOLD};
enum Facing {NONE, NORTH, EAST, SOUTH, WEST};

class GameSquare
{
	private:
		string name;
		Type type;
		Facing facing;
		bool breeze;
		bool stench;

	public:
		string getName()
		{ return name; }

		Type getType()
		{ return type; }

		Facing getFacing()
		{ return facing; }

		bool getBreeze()
		{ return breeze; }

		bool getStench()
		{ return stench; }

		void setName(string name)
		{ this->name = name; }

		void setType(Type type)
		{ this->type = type; }

		void setFacing(Facing facing)
		{ this->facing = facing; }

		void setBreeze(bool breeze)
		{ this->breeze = breeze; }

		void setStench(bool stench)
		{ this->stench = stench; }

		void swapSquares(GameSquare &obj)
		{
			// set tmp to player
			GameSquare tmp;
			tmp.setName(this->name);
			tmp.setFacing(this->facing);
			tmp.setType(this->type);

			// set player to square
			this->name = obj.getName();
			this->facing = obj.getFacing();
			this->type = obj.getType();

			// set square to tmp
			obj.setName(tmp.getName());
			obj.setFacing(tmp.getFacing());
			obj.setType(tmp.getType());
		}

};

#endif
