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

    void GenerateUniqueGameStates();
    void PerformMove(std::set<Game*>& uniqueStates, BoardCell* cell);
};


#endif //GIPF_SOLVER_H
