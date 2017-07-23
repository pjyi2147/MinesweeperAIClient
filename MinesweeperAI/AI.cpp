#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <list>

#include "json.hpp"
#include "minesweeper.h"


using namespace std;
using json = nlohmann::json;

string doubleClick(int col, int row)
{
	stringstream s;
	s << "D " << to_string(col) << " " << to_string(row);
	return s.str();
}

string simpleClick(int col, int row)
{
	stringstream s;
	s << "E " << to_string(col) << " " << to_string(row);
	return s.str();
}

string flagClick(int col, int row)
{
	stringstream s;
	s << "F " << to_string(col) << " " << to_string(row);
	return s.str();
}

void basicFlagging(MineSweeper* m, vector<string>* orders)
{
	for (int r = 0; r < m->returnRow(); ++r)
	{
		for (int c = 0; c < m->returnCol(); ++c)
		{
			// when the tile is done or covered, then continue;
			if (m->returnDone(c, r) || m->returnCovered(c, r))
				continue;

			// the number of covered is the same as the neighborcount
			if (m->returnNeighborCount(c, r) != 0 && m->returnNeighborCount(c, r) == m->countCovered(c, r))
			{
				// to examine 8 adjacent tiles
				for (int yoff = -1; yoff <= 1; ++yoff)
				{
					for (int xoff = -1; xoff <= 1; ++xoff)
					{
						if (xoff != 0 || yoff != 0)
						{
							int col = c + xoff;
							int row = r + yoff;

							if (col > -1 && col < m->returnCol()
								&& row > -1 && row < m->returnRow())
							{
								// covered and not flagged 
								// flag the tile!
								if (m->returnCovered(col, row) && !m->returnFlagged(col, row))
								{
									orders->push_back(flagClick(col, row));

									cout << "Flagged col: " << col << " row: " << row << endl;
									m->setFlag(col, row);
									// set done
									m->setDone(col, row, true);
								}
							}
						}
					}
				}
			}
		}
	}
}

void basicDoubleClicking(MineSweeper* m, vector<string>* orders)
{
	for (int r = 0; r < m->returnRow(); ++r)
	{
		for (int c = 0; c < m->returnCol(); ++c)
		{
			// when the tile is done or covered, then continue;
			if (m->returnDone(c, r) || m->returnCovered(c, r))
				continue;

			// the number of flagged is the same as the neighborcount
			if (m->returnNeighborCount(c, r) == m->countFlag(c, r))
			{
				orders->push_back(doubleClick(c, r));

				cout << "Double-Clicked col: " << c << " row: " << r << endl;
				m->setDone(c, r, true);
			}
		}
	}
}

vector<int> getBorderTiles(MineSweeper* m)
{
	vector<int> borderTiles;
	for (int r = 0; r < m->returnRow(); ++r)
	{
		for (int c = 0; c < m->returnCol(); ++c)
		{
			// when the tile is done or revealed or flagged, then continue;
			if (m->returnDone(c, r) || !m->returnCovered(c, r) || m->returnFlagged(c, r))
				continue;

			// examine 8 adjacent tiles
			for (int yoff = -1; yoff <= 1; ++yoff)
			{
				bool border = false;
				for (int xoff = -1; xoff <= 1; ++xoff)
				{
					if (xoff != 0 || yoff != 0)
					{
						int col = c + xoff;
						int row = r + yoff;

						// should not be out of bounds 
						if (col > -1 && col < m->returnCol()
							&& row > -1 && row < m->returnRow())
						{
							if (!m->returnCovered(col, row))
							{
								borderTiles.push_back(r*m->returnCol() + c);
								border = true;
								break;
							}
						}
					}
				}
				if (border) break;
			}
		}
	}
	return borderTiles;
}

vector<int> getEmptyTiles(MineSweeper* m)
{
	vector<int> EmptyTiles;
	for (int r = 0; r < m->returnRow(); ++r)
	{
		for (int c = 0; c < m->returnCol(); ++c)
		{
			if (!m->returnFlagged(c, r) && m->returnCovered(c, r))
			{
				EmptyTiles.push_back(r*m->returnCol() + c);
			}
		}
	}
	return EmptyTiles;
}

vector< vector<int> > getConnectedBorderTiles(MineSweeper *m, vector<int> borderTiles)
{
	vector< vector<int> > k;
	vector<int> covered;

	while (true)
	{
		list<int> queue;
		vector<int> finishedRegion;

		for (auto& firstT: borderTiles)
		{
			if (std::find(covered.begin(), covered.end(), firstT) == covered.end()) {
				queue.push_back(firstT);
				break;
			}
		}
		
		if (queue.size() == 0)
			break;

		while(queue.size() != 0)
		{
			int curTile = queue.front();
			queue.pop_front();

			int curCol = curTile % m->returnCol();
			int curRow = curTile / m->returnCol();
			
			finishedRegion.push_back(curTile);
			covered.push_back(curTile);

			for (auto& tile: borderTiles)
			{
				int tileCol = tile % m->returnCol();
				int tileRow = tile / m->returnCol();

				bool isConnected = false;
				
				if (std::find(finishedRegion.begin(), finishedRegion.end(), tile) != finishedRegion.end())
					continue;

				// the distance should be less than 2 to have common open tile 
				if (abs(curCol - tileCol) <= 2 || abs(curRow - tileRow) <= 2)
				{
					for (int r = 0; r < m->returnRow(); ++r)
					{
						for (int c = 0; c < m->returnCol(); ++c)
						{
							if (!m->returnCovered(c, r))
							{
								if (abs(curCol - c) <= 1 && abs(curRow - r) <= 1 &&
									abs(tileCol - c) <= 1 && abs(tileRow - r) <= 1)
								{
									isConnected = true;
									break;
								}
							}
						}
						if (isConnected) break;
					}
				}

				if (!isConnected) continue;

				if (find(queue.begin(), queue.end(), tile) == queue.end())
					queue.push_back(tile);
			}
		}
		k.push_back(finishedRegion);
	}
	return k;
}

void randomGuess(MineSweeper* m, vector<string>* orders)
{
	vector<int> borderTiles = getBorderTiles(m);
	srand(time(NULL));
	int k = borderTiles[rand() % borderTiles.size()];
	int col = k % m->returnCol(), row = k / m->returnCol();
	orders->push_back(simpleClick(col, row));
	cout << "AI guessed Col: " << col << " Row: " << row << " randomly from bordertiles" << endl;
}

void tankRecurse(vector<int> section, MineSweeper* m, int k, bool borderOptimization, vector<map<int, bool>>* solutions, int* test)
{
	++(*test);
	int flagCount = m->countAllFlagged();
	int mineCol = m->returnCol(), mineRow = m->returnRow();
	// total flagcount cannot exceed the total mineNum
	if (flagCount > m->returnMineNum()) return;
	
	// if the flagcount is bigger than the neighborcount already?
	// then, return before it goes through more recursion.
	for (auto& tile1 : section)
	{
		int tCol1 = tile1 % mineCol, tRow1 = tile1 / mineCol;
		for (int yoff = -1; yoff <= 1; ++yoff)
		{
			for (int xoff = -1; xoff <= 1; ++xoff)
			{
				int c = tCol1 + xoff;
				int r = tRow1 + yoff;
				// within in the range
				if (c > -1 && c < mineCol &&
					r > -1 && r < mineRow)
				{
					// should be revealed
					if (!m->returnCovered(c, r))
					{
						// if the flagcount is bigger than the neighborcount already?
						// then, return before it goes through more recursion.
						if (m->countFlag(c, r) > m->returnNeighborCount(c, r))
							return;
					}
				}
			}
		}
	} 
	
	// DFS Search
	// go to depth k and check it is correct
	if (k == section.size())
	{
		for (auto& tile : section)
		{
			int tCol = tile % mineCol, tRow = tile / mineCol;
			for (int yoff = -1; yoff <= 1; ++yoff)
			{
				for (int xoff = -1; xoff <= 1; ++xoff)
				{
					int c = tCol + xoff;
					int r = tRow + yoff;
					// within in the range
					if (c > -1 && c < mineCol &&
						r > -1 && r < mineRow)
					{
						// should be revealed
						if (!m->returnCovered(c, r))
						{
							// if the counting is not the same?
							// then it is not the possible answer.
							if (m->countFlag(c, r) != m->returnNeighborCount(c, r))
								return;
						}
					}
				}
			}
		}

		if (!borderOptimization && flagCount == m->countAllFlagged())
			return;

		map<int, bool> solution;
		for (auto& i : section)
		{
			int col = i % m->returnCol(), row = i / m->returnCol();
			solution.insert(make_pair(i, m->returnFlagged(col, row)));
		}
		solutions->push_back(solution);
		return;
	}

	int location = section[k];
	int qCol = location % m->returnCol();
	int qRow = location / m->returnCol();

	m->setFlag(qCol, qRow, true);
	tankRecurse(section, m, k + 1, borderOptimization, solutions, test);
	m->setFlag(qCol, qRow, false);
	tankRecurse(section, m, k + 1, borderOptimization, solutions, test);
}


void tankSolver(MineSweeper* m, int bruteforceSize = 8)
{
	auto borderTiles = getBorderTiles(m);
	auto emptyTiles = getEmptyTiles(m);
	int test = 0;
	vector< vector <int> > connectedTiles;
	vector< vector< map<int, bool> > > bigSolutions;
	bool borderOptimization = false;
	if (m->countAllCovered() - borderTiles.size() > bruteforceSize)
	{
		connectedTiles = getConnectedBorderTiles(m, borderTiles);
		borderOptimization = true;
	}
	else connectedTiles.push_back(emptyTiles);
	
	if (connectedTiles.size() == 0) {
		cout << "Something went wrong... there is no bordertiles" << endl;
		system("pause");
		return; // something is wrong...
	}

	for (int s = 0; s < connectedTiles.size(); ++s)
	{
		// not to cause a problem, create a copy of m
		vector< map <int, bool> > solutions;
		auto mcopy = MineSweeper(*m);
		if (connectedTiles[s].size() >= 15) cout << "This recursion will take lots of time..." << endl;
		tankRecurse(connectedTiles[s], &mcopy, 0, borderOptimization, &solutions, &test);
		bigSolutions.push_back(solutions);
	}

	cout << endl << "tankRecurse was called " << test << " times. " << endl << endl;

	return; // do nothing 
}

void AI(MineSweeper* m, json* to_send)
{
	// first is for flagging
	vector<string> orders;
	
	// basic flagging
	basicFlagging(m, &orders);

	// Double clicking
	basicDoubleClicking(m, &orders);

	if (orders.size() == 0)
	{
		cout << "Guesses start..." << endl;
		tankSolver(m);
		randomGuess(m, &orders);
		system("pause");
	}
	(*to_send)["orders"] = orders;
}