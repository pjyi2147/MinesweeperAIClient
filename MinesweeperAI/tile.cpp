#include <iostream>
#include <vector>

#include "tile.h"

using namespace std;
Tile::Tile()
{
	this->_col = 0;
	this->_row = 0;
	//this->_revealed = true;
}

Tile::Tile(int col, int row)
{
	this->_col = col;
	this->_row = row;
	//this->_revealed = true;
}

Tile::Tile(Tile& t)
{
	this->_mine = t.isMine();
	this->_revealed = t.isRevealed();
	this->_flagged = t.isFlagged();

	this->_neighborCount = t.returnNeighborCount();
	this->_done = t.isDone();
	
	this->_col = t.returnCol();
	this->_row = t.returnRow();
}

void Tile::printTile()
{
	std::cout << "|";
	if (this->isRevealed() && this->isMine()) cout << "M";
	else if (this->isRevealed())
		cout << this->returnNeighborCount();
	else if (this->isFlagged()) cout << "F";			// Flag
	else cout << "C";									// Covered 
}

void Tile::reveal()
{
	this->_revealed = true;
}

// is series
// is it mine?
bool Tile::isMine()
{
	return this->_mine;
}

// is it Flagged?
bool Tile::isFlagged()
{
	return this->_flagged;
}

// is it revealed?
bool Tile::isRevealed()
{
	return this->_revealed;
}

// is it done? (AI) 
bool Tile::isDone()
{
	return this->_done;
}

// return series
// return its column
int Tile::returnCol()
{
	return this->_col;
}

// return its row
int Tile::returnRow()
{
	return this->_row;
}

// return neighborCount;
int Tile::returnNeighborCount()
{
	return this->_neighborCount;
}

// set series
// set Mine
void Tile::setMine()
{
	this->_mine = true;
}

// set Flag
void Tile::setFlag()
{
	if (this->_flagged) this->_flagged = false;
	else this->_flagged = true;
}

void Tile::setFlag(bool set)
{
	this->_flagged = set;
}

// set NeighborCount
void Tile::setNeighborCount(int count)
{
	this->_neighborCount = count;
}

// set done
void Tile::setDone(bool done)
{
	this->_done = done;
}