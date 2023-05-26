#ifndef GIPF_GAME_H
#define GIPF_GAME_H

#include <list>
#include <vector>
#include "BoardCell.h"

#define in_progress 0
#define white_win 1
#define black_win 2
#define dead_lock_white 3
#define dead_lock_black 4
#define bad_move_white 5
#define bad_move_black 6

#define DIAGONAL_LENGTH 2 * boardSize - 1

class Game {
private:
    std::list<BoardCell*>* board;
    int boardSize;
    int pawnTakeThreshold;
    int whiteInitialPawns;
    int blackInitialPawns;

    int whiteOnBoard;
    int blackOnBoard;

    int whiteReserve;
    int blackReserve;

    bool currentPlayer;
    int gameState;

    [[nodiscard]] bool CheckGameConfigCorrectness() const;
    void InitBoardArray();
    void DeleteBoardArray();
    bool FillBoard();
    static void DetermineMoveCoords(std::string& move, int* xSource, int* ySource, int* xDest, int* yDest);
    bool CheckBadIndex(int x, int y);
    bool CheckBadMoveWrongField(int xSource, int ySource, int xDest, int yDest);
public:
    Game(int boardSize, int pawnTakeThreshold, int whiteInitialPawns, int blackInitialPawns, int whiteReserve, int blackReserve, bool currentPlayer);
    ~Game();

    void PrintBoard();
    void MakeMove(std::vector<std::string>& arguments);
};


#endif //GIPF_GAME_H
