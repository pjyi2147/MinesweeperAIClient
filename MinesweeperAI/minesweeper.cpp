#include <iostream>
#include <vector>
#include "tile.h"
#include "minesweeper.h"
#include <ctime>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

MineSweeper::MineSweeper(int col, int row, int mineNum)
{
	this->_mineNum = mineNum;
	this->_col = col;
	this->_row = row;

	this->_minefield = new Tile*[row];
	for (int r = 0; r < row; ++r)
	{
		this->_minefield[r] = new Tile[col];
	}

	for (int r = 0; r < row; ++r)
	{
		for (int c = 0; c < col; ++c)
		{
			this->_minefield[r][c] = Tile(c, r);
		}
	}
}

MineSweeper::MineSweeper(MineSweeper& m)
{
	this->_mineNum = m.returnMineNum();
	this->_col = m.returnCol();
	this->_row = m.returnRow();

	this->_minefield = new Tile*[_row];
	for (int r = 0; r < _row; ++r)
	{
		this->_minefield[r] = new Tile[_col];
	}

	for (int r = 0; r < _row; ++r)
	{
		for (int c = 0; c < _col; ++c)
		{
			this->_minefield[r][c] = Tile(m.returnTile(c, r));
		}
	}
}

MineSweeper::~MineSweeper()
{
	for (int r = 0; r < this->returnRow(); ++r)
	{
		delete[] this->_minefield[r];
	}
	delete[] this->_minefield;
}


void MineSweeper::update(json info)
{
	this->setGameEnd(info["GameEnd"]);
	this->setWin(info["win"]);
	string mineState = info["mineField"];
	for (int i = 0; i < mineState.length(); ++i)
	{
		int row = i / this->returnCol();
		int col = i % this->returnCol();
		// when numbers
		char k = mineState[i];
		if (isdigit(k))
		{
			this->_minefield[row][col].setNeighborCount(atoi(&k));
			this->_minefield[row][col].reveal();
			if (this->_minefield[row][col].returnNeighborCount() == 0)
			{
				this->_minefield[row][col].setDone(true);
			}
		}
		// when flagged;
		else if (k == 'F')
		{
			if (!this->_minefield[row][col].isFlagged())
			{
				cout << "Something is wrong" << endl;
			} 
			// do nothing
		}
		// when covered;
		else if (k == 'C')
		{
			// do nothing 
		}
		// when mined;
		else if (k == 'M')
		{
			this->_minefield[row][col].reveal();
			this->_minefield[row][col].setMine();
		}
	}
}

void MineSweeper::createMinefield(int col, int row)
{
	vector<pair<int, int>> options;

	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (!(r == row && c == col))
			{
				options.push_back(make_pair(c, r));
			}
		}
	}

	for (int i = 0; i < this->returnMineNum(); ++i)
	{
		if (options.size() > 0)
		{
			srand(time(NULL));
			int k = (rand() % options.size());
			auto pair1 = options[k];
			this->_minefield[pair1.second][pair1.first].setMine();
			options.erase(options.begin() + k);;
		}
	}

	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->countMine(c, r);
		}
	}

	this->revealTile(col, row);
}

void MineSweeper::checkWin()
{
	bool win = true;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (!this->_minefield[r][c].isRevealed()
				&& !this->_minefield[r][c].isMine())
				win = false;
		}
	}
	if (win)
	{
		this->setGameEnd(true);
		this->setWin(true);
		this->EndGame(true);
	}
}

void MineSweeper::printMineField()
{
	for (int c = 0; c < this->returnCol() * 2 + 1; ++c)
	{
		cout << "-";
	}
	cout << endl;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->_minefield[r][c].printTile();
		}
		cout << "|" << endl;
		for (int c = 0; c < this->returnCol() * 2 + 1; ++c)
		{
			cout << "-";
		}
		cout << endl;
	}
}

void MineSweeper::revealTile(int col, int row)
{

	this->_minefield[row][col].reveal();
	if (this->_minefield[row][col].isMine())
	{
		this->setGameEnd(true);
		this->EndGame(false);
	}
	if (this->_minefield[row][col].returnNeighborCount() == 0
		&& !this->_minefield[row][col].isMine())
	{
		for (int yoff = -1; yoff <= 1; ++yoff)
		{
			for (int xoff = -1; xoff <= 1; ++xoff)
			{
				int c = col + xoff;
				int r = row + yoff;

				if (c > -1 && c < this->returnCol() && r > -1
					&& r < this->returnRow())
				{
					if (!this->_minefield[r][c].isRevealed())
						this->revealTile(c, r);
				}
			}
		}
	}
}

void MineSweeper::revealDoubleClick(int col, int row)
{
	auto tile = this->_minefield[row][col];
	// not revealed -> do nothing
	if (!tile.isRevealed()) return;

	// the flag count does not match the mine num -> do nothing
	if (this->countFlag(col, row) != tile.returnNeighborCount()) return;

	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int c = col + xoff;
				int r = row + yoff;

				// within range
				if (c > -1 && c < this->returnCol() && r > -1
					&& r < this->returnRow())
				{
					if (!this->_minefield[r][c].isFlagged())
						this->revealTile(c, r);
				}
			}
		}
	}
}

void MineSweeper::countMine(int col, int row)
{
	// if the tile is mine
	if (this->_minefield[row][col].isMine())
	{
		this->_minefield[row][col].setNeighborCount(-1);
		return;
	}

	// not mine
	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			int c = col + xoff;
			int r = row + yoff;

			if (c > -1 && c < this->returnCol()
				&& r > -1 && r < this->returnRow())
			{
				if (this->_minefield[r][c].isMine()) total += 1;
			}
		}
	}
	this->_minefield[row][col].setNeighborCount(total);
}

int MineSweeper::countFlag(int col, int row)
{
	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int c = col + xoff;
				int r = row + yoff;

				if (c > -1 && c < this->returnCol()
					&& r > -1 && r < this->returnRow())
				{
					if (this->_minefield[r][c].isFlagged()) total += 1;
				}
			}
		}
	}
	return total;
}

int MineSweeper::countCovered(int col, int row)
{
	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int c = col + xoff;
				int r = row + yoff;

				if (c > -1 && c < this->returnCol()
					&& r > -1 && r < this->returnRow())
				{
					if (!this->_minefield[r][c].isRevealed()) total += 1;
				}
			}
		}
	}
	return total;
}

int MineSweeper::countAllCovered()
{
	int total = 0;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (!this->_minefield[r][c].isRevealed())
				++total;
		}
	}
	return total;
}

int MineSweeper::countAllFlagged()
{
	int total = 0;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (this->_minefield[r][c].isFlagged())
				++total;
		}
	}
	return total;
}

void MineSweeper::EndGame(bool win)
{
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->_minefield[r][c].reveal();
		}
	}

	this->printMineField();

	if (win)
	{
		cout << "Won!" << endl;
	}
	else
	{
		cout << "Lost..." << endl;
	}

	this->setGameEnd(true);
}


// set series
// set GameEnd
void MineSweeper::setGameEnd(bool finished)
{
	this->_gameEnd = true;
}

void MineSweeper::setFlag(int col, int row)
{
	if (!this->_minefield[row][col].isRevealed())
		this->_minefield[row][col].setFlag();
	else cout << "You cannot flag tiles that are already open" << endl;
}

void MineSweeper::setFlag(int col, int row, bool set)
{
	if (!this->_minefield[row][col].isRevealed())
		this->_minefield[row][col].setFlag(set);
	else cout << "You cannot flag tiles that are already open" << endl;
}
void MineSweeper::setWin(bool win)
{
	this->_win = win;
}

void MineSweeper::setDone(int col, int row, bool done)
{
	this->_minefield[row][col].setDone(done);
}

// return series
// return MineNum
int MineSweeper::returnMineNum()
{
	return this->_mineNum;
}

// return columns
int MineSweeper::returnCol()
{
	return this->_col;
}

// return row
int MineSweeper::returnRow()
{
	return this->_row;
}

// return gameEnd
bool MineSweeper::returnGameEnd()
{
	return this->_gameEnd;
}

// return win
bool MineSweeper::returnWin()
{
	return this->_win;
}

// return done
bool MineSweeper::returnDone(int col, int row)
{
	return this->_minefield[row][col].isDone();
}

bool MineSweeper::returnCovered(int col, int row)
{
	return !this->_minefield[row][col].isRevealed();
}

int MineSweeper::returnNeighborCount(int col, int row)
{
	return this->_minefield[row][col].returnNeighborCount();
}

bool MineSweeper::returnFlagged(int col, int row)
{
	return this->_minefield[row][col].isFlagged();
}

Tile MineSweeper::returnTile(int col, int row)
{
	return this->_minefield[row][col];
}