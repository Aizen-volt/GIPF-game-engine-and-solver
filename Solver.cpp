#include "Solver.h"
#include "Game.h"
#include <set>
#include <iostream>
#include <numeric>


Solver::Solver(Game *game) {
    originalGame = game;
    boardSize = game->GetBoardSize();
}


Solver::~Solver() {
    delete gameCopy;
}


void Solver::GenerateUniqueGameStates(bool verbal, bool winOnly) {
    gameCopy = new Game(originalGame);
    std::set<Game*> uniqueStates;
    std::list<BoardCell*>::iterator cell;

    int exitCode;

    for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
        if (i == 0 || i == DIAGONAL_LENGTH + 1) {
            for (int j = 0; j < boardSize + 1; j++) {
                cell = gameCopy->GetBoard()[i].begin();
                std::advance(cell, j);
                exitCode = PerformMove(uniqueStates, *cell, verbal, winOnly);
                if (exitCode == 1)
                    return;
            }
        }
        else {
            cell = gameCopy->GetBoard()[i].begin();
            exitCode = PerformMove(uniqueStates, *cell, verbal, winOnly);
            if (exitCode == 1)
                return;

            cell = gameCopy->GetBoard()[i].end();
            std::advance(cell, -1);
            exitCode = PerformMove(uniqueStates, *cell, verbal, winOnly);
            if (exitCode == 1)
                return;
        }
    }

    if (verbal) {
        for (auto& state : uniqueStates)
            state->PrintBoard();
    }
    else
        std::cout << uniqueStates.size() << "_UNIQUE_MOVES\n";

    for (auto& state : uniqueStates)
        delete state;
}

int Solver::PerformMove(std::set<Game*>& uniqueStates, BoardCell *cell, bool verbal, bool winOnly) {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> unclearRows;

    for (auto & foundConnection : cell->foundConnections) {
        std::string argument =
                char(foundConnection.first.second + 'a') + std::to_string(foundConnection.first.first + 1) + '-' +
                char(foundConnection.second.second + 'a') + std::to_string(foundConnection.second.first + 1);
        std::vector<std::string> temp;
        temp.push_back(argument);
        unclearRows = gameCopy->MakeMove(temp, false);
        if (!unclearRows.empty()) {
            for (const auto & unclearRow : unclearRows) {
                gameCopy->CaptureRow(unclearRow);
                CheckUniqueness(uniqueStates);
                gameCopy->MakeMove(temp, false);
            }
            gameCopy = new Game(originalGame);
            return 0;
        }

        if (winOnly && (gameCopy->GetGameState() == "white_win" || gameCopy->GetGameState() == "black_win" ||
                        gameCopy->GetGameState() == "dead_lock black" ||
                        gameCopy->GetGameState() == "dead_lock white")) {
            std::vector<std::pair<std::vector<BoardCell*>, bool>> rowsToCapture = gameCopy->GetRowsToCapture();
            if (!rowsToCapture.empty())
                gameCopy->CaptureRow(rowsToCapture[0]);
            if (verbal)
                gameCopy->PrintBoard();
            else
                std::cout << "1_UNIQUE_MOVES\n";
            return 1;
        }

        CheckUniqueness(uniqueStates);
    }


    return 0;
}


void Solver::CheckUniqueness(std::set<Game*>& uniqueStates) {
    if (gameCopy->GetGameState() == "in_progress" || gameCopy->GetGameState() == "white_win" || gameCopy->GetGameState() == "black_win" || (gameCopy->GetGameState() == "dead_lock black" && gameCopy->GetCurrentPlayer() == BLACK) || gameCopy->GetGameState() == "dead_lock white" && gameCopy->GetCurrentPlayer() == WHITE) {
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