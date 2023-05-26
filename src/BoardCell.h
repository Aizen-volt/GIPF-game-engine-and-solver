#ifndef GIPF_BOARDCELL_H
#define GIPF_BOARDCELL_H

#define WHITE 0
#define BLACK 1
#define EMPTY 2
#define BORDER 3

class BoardCell {
private:
    int state;
public:
    int x, y;
    BoardCell(int x, int y, int state);

    int GetState() const;
    void SetState(int state);
};


#endif //GIPF_BOARDCELL_H
