#ifndef GIPF_BOARDCELL_H
#define GIPF_BOARDCELL_H

#include <map>

#define WHITE 0
#define BLACK 1
#define EMPTY 2
#define BORDER 3

class BoardCell {
private:
    int state;
public:
    int x, y;
    std::multimap<std::pair<int, int>, std::pair<int, int>> foundConnections;
    BoardCell(int x, int y, int state);

    int GetState() const;
    void SetState(int state);
};


#endif //GIPF_BOARDCELL_H
