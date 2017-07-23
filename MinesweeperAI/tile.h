#pragma once

class Tile
{
private:
	bool _mine = false;				// is it mine?
	bool _revealed = false;			// is it revealed?
	bool _flagged = false;

	int _neighborCount = 0;				// how many mines does the neighbors have?
	int _col;
	int _row;
	bool _done = false;

public:
	Tile();
	Tile(Tile& t);
	Tile(int col, int row);

	void reveal();
	void printTile();

	bool isMine();
	bool isRevealed();
	bool isFlagged();
	bool isDone();

	int returnRow();
	int returnCol();
	int returnNeighborCount();

	void setMine();
	void setFlag();
	void setFlag(bool set);
	void setNeighborCount(int count);
	void setDone(bool done);
};