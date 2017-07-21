#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "json.hpp"
#include "minesweeper.h"

using namespace std;
using json = nlohmann::json;

void AI(MineSweeper* m, json* to_send)
{
	// first is for flagging
	vector<string> orders;
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
								if (m->returnCovered(col, row) && !m->returnFlagged(col, row))
								{
									stringstream s;
									s << "F " << to_string(col) << " " << to_string(row);
									orders.push_back(s.str());

									cout << "Flagged col: " << col << " row: " << row << endl;
									m->setFlag(col, row);
									m->setDone(col, row, true);
								}
							}
						}
					}
				}
			}
		}
	}
	// Double clicking
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
				stringstream s;
				s << "D " << to_string(c) << " " << to_string(r);
				orders.push_back(s.str());

				cout << "Double-Clicked col: " << c << " row: " << r << endl;
				m->setDone(c, r, true);
				// m->revealDoubleClick(c, r);
			}
		}
	}

	if (orders.size() == 0)
	{
		cout << "Can't solve this minesweeper yet..." << endl;
		system("pause");
	}
	else
	{
		(*to_send)["orders"] = orders;
	}
}