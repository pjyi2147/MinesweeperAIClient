// MinesweeperAI.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "minesweeper.h"
#include "client.h"
#include "json.hpp"
#include "AI.h"

using namespace std;
using json = nlohmann::json;

void GameLoop(json* info, json* to_send, MineSweeper *m)
{
	m->printMineField();
	AI(m, to_send);
	ingameTransfer(to_send, info, m);
}

int main()
{
	cout << "Minesweeper AI" << endl << endl;
	int col, row, mineNum;
	startGamefunction(col, row, mineNum);
	auto minefield = MineSweeper(col, row, mineNum);

	json info, to_send;
	firstScriptTransfer(col, row, info);
	minefield.update(info);
	while (!info["GameEnd"])
	{
		GameLoop(&info, &to_send, &minefield);
	}

	if (info["win"])
	{
		minefield.printMineField();
		cout << "won!" << endl;
	}
	else
	{
		minefield.printMineField();
		cout << "lost..." << endl;
	}

	system("pause");
	return 0;
}