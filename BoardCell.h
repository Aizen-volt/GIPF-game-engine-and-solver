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
    multimap<Position, Position> connections; // first - position of the cell that stone came from,
                                              // second - position of the cell that stone should go to while pushing


    BoardCell(Position position, State state) : position(position), state(state) {}
};


#endif //GIPF_BOARDCELL_H
