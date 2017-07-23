#pragma once
#include <iostream>
#include <vector>

#include "tile.h"
#include "json.hpp"

using namespace std;

class MineSweeper
{
private:
	int _col;
	int _row;
	int _mineNum;
	bool _win = false;
	bool _done = false;
	Tile** _minefield;

	bool _gameEnd = false;

public:
	MineSweeper(int col, int row, int mineNum);
	MineSweeper(MineSweeper& m);
	~MineSweeper();

	void update(nlohmann::json info);

	void countMine(int col, int row);
	int countFlag(int col, int row);
	int countCovered(int col, int row);
	int countAllFlagged();
	int countAllCovered();

	void createMinefield(int col, int row);
	void printMineField();

	void revealTile(int col, int row);
	void revealDoubleClick(int col, int row);

	void EndGame(bool win);
	void checkWin();

	void setGameEnd(bool finished);
	void setFlag(int col, int row);
	void setFlag(int col, int row, bool flag);
	void setWin(bool win);
	void setDone(int col, int row, bool done);

	bool returnGameEnd();
	int returnCol();
	int returnRow();
	int returnMineNum();
	bool returnWin();
	bool returnDone(int col, int row);
	bool returnCovered(int col, int row);
	int returnNeighborCount(int col, int row);
	bool returnFlagged(int col, int row);
	Tile returnTile(int col, int row);
};
