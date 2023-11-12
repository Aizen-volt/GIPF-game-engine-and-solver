#ifndef GIPF_GAME_ENGINE_AND_SOLVER_POSITION_H
#define GIPF_GAME_ENGINE_AND_SOLVER_POSITION_H

class Position {
public:
    int x, y;


    Position(int x, int y) : x(x), y(y) {}


    Position(const Position& other) {
        x = other.x;
        y = other.y;
    }



    Position& operator=(const Position& other) {
        x = other.x;
        y = other.y;
        return *this;
    }


    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }


    bool operator<(const Position& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }


    bool operator>(const Position& other) const {
        return x > other.x || (x == other.x && y > other.y);
    }


    bool operator<=(const Position& other) const {
        return x <= other.x || (x == other.x && y <= other.y);
    }


    bool operator>=(const Position& other) const {
        return x >= other.x || (x == other.x && y >= other.y);
    }


    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }


    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }
};

#endif //GIPF_GAME_ENGINE_AND_SOLVER_POSITION_H
