#pragma once

void AI(MineSweeper* m, nlohmann::json* to_send);

void basicFlagging(MineSweeper* m, vector<string>* orders);
void basicDoubleClicking(MineSweeper* m, vector<string>* orders);

void tankSolver(MineSweeper* m, int bruteforcesize);
void tankRecurse(vector<int> section, MineSweeper* m, bool borderOptimization, vector<map<int, bool>>* solutions, int*);
void randomGuess(MineSweeper* m, vector<string>* orders);

vector<int> getBorderTiles(MineSweeper* m);
vector<int> getEmptyTiles(MineSweeper* m);
vector<vector<int>> getConnectedBorderTiles(MineSweeper *m);

string doubleClick(int col, int row);
string flagClick(int col, int row);
string simpleClick(int col, int row);