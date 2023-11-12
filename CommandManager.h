#ifndef GIPF_COMMANDMANAGER_H
#define GIPF_COMMANDMANAGER_H

#include "Game.h"
#include "Solver.h"

class CommandManager {
private:
    Game* game;
    Solver* solver;

    void CreateGame();
public:
    void DispatchCommand();
};


#endif //GIPF_COMMANDMANAGER_H
