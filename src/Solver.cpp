#include "Solver.h"
#include "Game.h"
#include <set>
#include <iostream>
#include <numeric>


Solver::Solver(Game *game) {
    originalGame = game;
    boardSize = game->GetBoardSize();
}


void Solver::GenerateUniqueGameStates() {
    gameCopy = new Game(originalGame);
    std::set<Game*> uniqueStates;
    std::list<BoardCell*>::iterator cell;

    for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
        if (i == 0 || i == DIAGONAL_LENGTH + 1) {
            for (int j = 0; j < boardSize + 1; j++) {
                cell = gameCopy->GetBoard()[i].begin();
                std::advance(cell, j);
                PerformMove(uniqueStates, *cell);
            }
        }
        else {
            cell = gameCopy->GetBoard()[i].begin();
            PerformMove(uniqueStates, *cell);

            cell = gameCopy->GetBoard()[i].end();
            std::advance(cell, -1);
            PerformMove(uniqueStates, *cell);
        }
    }

    std::cout << uniqueStates.size() << "_UNIQUE_MOVES\n";
}

void Solver::PerformMove(std::set<Game*>& uniqueStates, BoardCell *cell) {
    for (auto & foundConnection : cell->foundConnections) {
        std::string argument = char(foundConnection.first.second + 'a') + std::to_string(foundConnection.first.first + 1) + '-' +
                char(foundConnection.second.second + 'a') + std::to_string(foundConnection.second.first + 1);
        std::vector<std::string> temp;
        temp.push_back(argument);
        gameCopy->MakeMove(temp, false);


        if (gameCopy->GetGameState() == "in_progress") {
            if (uniqueStates.empty())
                uniqueStates.emplace(gameCopy);

            std::vector<bool> differs(uniqueStates.size(), false);
            int counter = 0;
            for (auto& state : uniqueStates) {
                if (state->whiteReserve != gameCopy->whiteReserve || state->blackReserve != gameCopy->blackReserve) {
                    differs[counter] = true;
                    counter++;
                    continue;
                }

                for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
                    int k = 0;
                    for (auto it = state->GetBoard()[i].begin(); it != state->GetBoard()[i].end(); it++) {
                        auto it2 = gameCopy->GetBoard()[i].begin();
                        std::advance(it2, k);
                        k++;
                        if ((*it)->GetState() != (*it2)->GetState())
                            differs[counter] = true;
                    }
                }
                counter++;
            }
            if (std::accumulate(differs.begin(), differs.end(), 0) == differs.size())
                uniqueStates.emplace(gameCopy);
            gameCopy = new Game(originalGame);
        }
        else
            gameCopy = new Game(originalGame);
    }
}
