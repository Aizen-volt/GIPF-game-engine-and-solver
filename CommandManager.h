#ifndef GIPF_COMMANDMANAGER_H
#define GIPF_COMMANDMANAGER_H

#include "Game.h"
#include "Solver.h"

using std::cin;
using std::string;
using std::vector;
using std::stringstream;

class CommandManager {
private:
    Game* game;
    Solver* solver;

    // create game and solver from input
    void createGame() {
        // delete previous game
        if (game) {
            delete game;
            delete solver;
        }

        int boardSize, pawnTakeThreshold,
            whiteInitialPawns, blackInitialPawns,
            whiteReserve, blackReserve;
        char currentPlayer;

        // read game info
        cin >> boardSize >> pawnTakeThreshold
            >> whiteInitialPawns >> blackInitialPawns
            >> whiteReserve >> blackReserve
            >> currentPlayer;

        game = new Game(boardSize, pawnTakeThreshold, whiteInitialPawns, blackInitialPawns, whiteReserve, blackReserve, currentPlayer == 'B');
        solver = new Solver(game);
    }

public:
    void execute(const string& command) {
        // load new game state
        if (command == "LOAD_GAME_BOARD") {
            createGame();
        }

        // print board for current state
        else if (command == "PRINT_GAME_BOARD") {
            game->printBoard();
        }

        // try to make a move
        else if (command == "DO_MOVE") {
            string line, temp;

            getline(cin, line);
            line.erase(line.begin()); // remove space at the beginning

            vector<string> arguments; // <x1-x2> [w:|b:] y1 yn

            // separate arguments
            stringstream ss(line);
            while (std::getline(ss, temp, ' ')) {
                arguments.push_back(temp);
            }

            game->makeMove(arguments, true);
        }

        // print board for all possible moves
        else if (command == "GEN_ALL_POS_MOV") {
            solver->generateUniqueGameStates(true, false);
        }

        // print number of all possible moves
        else if (command == "GEN_ALL_POS_MOV_NUM") {
            solver->generateUniqueGameStates(false, false);
        }

        // print board for all possible moves, if there is a win print only that one
        else if (command == "GEN_ALL_POS_MOV_EXT") {
            solver->generateUniqueGameStates(true, true);
        }

        // print number of all possible moves, print 1 if there is a win
        else if (command == "GEN_ALL_POS_MOV_EXT_NUM") {
            solver->generateUniqueGameStates(false, true);
        }

        // print game state
        else if (command == "IS_GAME_OVER") {
            game->PrintGameState();
        }
    }
};

#endif //GIPF_COMMANDMANAGER_H
