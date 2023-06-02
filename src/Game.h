#ifndef GIPF_GAME_H
#define GIPF_GAME_H

#include <list>
#include <vector>
#include <string>
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
    int boardCellsCount;
    int pawnTakeThreshold;
    int whiteInitialPawns;
    int blackInitialPawns;

    int whiteOnBoard;
    int blackOnBoard;

    bool currentPlayer;
    std::string gameState;

    void InitBoardArray();
    void DeleteBoardArray();
    bool FillBoard();
    void FindCellConnections();
    static void DetermineMoveCoords(std::string& move, int* xSource, int* ySource, int* xDest, int* yDest);
    bool CheckBadIndex(int x, int y, bool verbal);
    bool CheckBadMoveWrongField(int xSource, int ySource, int xDest, int yDest, bool verbal);
    bool CheckUnknownDirection(int xSource, int ySource, int xDest, int yDest, bool verbal);
    bool CheckBadMoveRowFull(int xSource, int ySource, int xDest, int yDest, bool verbal);
    void MoveLine(int xSource, int ySource, int xDest, int yDest);
    int CheckRowsToCapture();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesDownLeftUpRight();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesDownRightUpLeft();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesLeftRight();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> GetRowsToCapture();
public:
    int whiteReserve;
    int blackReserve;

    Game(int boardSize, int pawnTakeThreshold, int whiteInitialPawns, int blackInitialPawns, int whiteReserve, int blackReserve, bool currentPlayer);
    explicit Game(Game* other);
    ~Game();


    void PrintBoard();
    void MakeMove(std::vector<std::string>& arguments, bool verbal);

    int GetBoardSize() const;
    [[nodiscard]] std::list<BoardCell*>* GetBoard() const;
    std::string GetGameState() const;
    void PrintGameState() const;
    bool GetCurrentPlayer() const;
};


#endif //GIPF_GAME_H
