#ifndef GIPF_BOARDCELL_H
#define GIPF_BOARDCELL_H

#include <map>
#include "Position.h"

using std::multimap;

class BoardCell {
public:
    enum State {
        white = 0,
        black = 1,
        empty = 2,
        border = 3
    };
    State state;
    Position position;
    multimap<Position, Position> connections; // stores cells that are reachable from this cell


    BoardCell(Position position, State state) : position(position), state(state) {}
};


#endif //GIPF_BOARDCELL_H
