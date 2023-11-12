#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "CommandManager.h"

void CommandManager::DispatchCommand() {
    std::string command;
    while (std::cin >> command) {
        if (command == "LOAD_GAME_BOARD")
            CreateGame();
        else if (command == "PRINT_GAME_BOARD")
            game->PrintBoard();
        else if (command == "DO_MOVE") {
            std::string line, temp;
            getline(std::cin, line);
            line.erase(line.begin());
            std::vector<std::string> arguments;
            std::stringstream ss(line);
            while (std::getline(ss, temp, ' ')) {
                arguments.push_back(temp);
            }
            game->MakeMove(arguments, true);
        }
        else if (command == "GEN_ALL_POS_MOV")
            solver->GenerateUniqueGameStates(true, false);
        else if (command == "GEN_ALL_POS_MOV_NUM")
            solver->GenerateUniqueGameStates(false, false);
        else if (command == "GEN_ALL_POS_MOV_EXT")
            solver->GenerateUniqueGameStates(true, true);
        else if (command == "GEN_ALL_POS_MOV_EXT_NUM")
            solver->GenerateUniqueGameStates(false, true);
        else if (command == "IS_GAME_OVER")
            game->PrintGameState();
    }
}

void CommandManager::CreateGame() {
    int boardSize, pawnTakeThreshold, whiteInitialPawns, blackInitialPawns, whiteReserve, blackReserve;
    char currentPlayer;
    std::cin >> boardSize >> pawnTakeThreshold >> whiteInitialPawns >> blackInitialPawns >> whiteReserve >> blackReserve >> currentPlayer;
    game = new Game(boardSize, pawnTakeThreshold, whiteInitialPawns, blackInitialPawns, whiteReserve, blackReserve, currentPlayer == 'B');
    solver = new Solver(game);
}