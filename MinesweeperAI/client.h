#pragma once
#include "json.hpp"

void client();
void startGamefunction(int &col, int &row, int &mineNum);
void firstScriptTransfer(int col, int row, nlohmann::json& info);
void ingameTransfer(nlohmann::json* to_send, nlohmann::json* info, MineSweeper* m);