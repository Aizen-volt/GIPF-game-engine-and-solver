#ifndef GIPF_SOLVER_H
#define GIPF_SOLVER_H

#include <set>
#include "Game.h"

class Solver {
private:
    Game* originalGame;
    Game* gameCopy;
    int boardSize;
public:
    Solver(Game* game);

    void GenerateUniqueGameStates(bool verbal, bool winOnly);
    int PerformMove(std::set<Game*>& uniqueStates, BoardCell* cell, bool verbal, bool winOnly);
};


#endif //GIPF_SOLVER_H
