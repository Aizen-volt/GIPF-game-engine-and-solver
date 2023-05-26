#include <string>
#include <iostream>
#include "CommandManager.h"

void CommandManager::DispatchCommand() {
    std::string command;
    while (std::cin >> command) {
        if (command == "LOAD_GAME_BOARD")
            CreateGame();
        else if (command == "PRINT_GAME_BOARD")
            game->PrintBoard();
    }
}

void CommandManager::CreateGame() {
    int boardSize, pawnTakeThreshold, whiteInitialPawns, blackInitialPawns, whiteReserve, blackReserve;
    char currentPlayer;
    std::cin >> boardSize >> pawnTakeThreshold >> whiteInitialPawns >> blackInitialPawns >> whiteReserve >> blackReserve >> currentPlayer;
    game = new Game(boardSize, pawnTakeThreshold, whiteInitialPawns, blackInitialPawns, whiteReserve, blackReserve, currentPlayer == 'B');
}