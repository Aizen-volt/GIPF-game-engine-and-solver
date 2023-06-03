#ifndef GIPF_SOLVER_H
#define GIPF_SOLVER_H

#include <set>
#include "Game.h"

class Solver {
private:
    Game* originalGame;
    Game* gameCopy;
    int boardSize;

    void CheckUniqueness(std::set<Game*>& uniqueStates);
public:
    explicit Solver(Game* game);
    ~Solver();

    void GenerateUniqueGameStates(bool verbal, bool winOnly);
    int PerformMove(std::set<Game*>& uniqueStates, BoardCell* cell, bool verbal, bool winOnly);
};


#endif //GIPF_SOLVER_H
