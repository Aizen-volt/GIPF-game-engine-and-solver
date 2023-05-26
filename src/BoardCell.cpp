#include "BoardCell.h"

BoardCell::BoardCell(int x, int y, int state) {
    this->x = x;
    this->y = y;
    this->state = state;
}

int BoardCell::GetState() const {
    return state;
}

void BoardCell::SetState(int state) {
    this->state = state;
}