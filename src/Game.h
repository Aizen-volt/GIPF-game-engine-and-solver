#ifndef GIPF_GAME_H
#define GIPF_GAME_H

#include <list>
#include <vector>
#include <string>
#include "BoardCell.h"

#define DIAGONAL_LENGTH (2 * boardSize - 1)


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
    void DeleteBoardArray() const ;
    bool FillBoard();
    void FindCellConnections();
    static void DetermineMoveCoords(std::string& move, int* xSource, int* ySource, int* xDest, int* yDest);
    bool CheckBadIndex(int x, int y, bool verbal);
    bool CheckBadMoveWrongField(int xSource, int ySource, int xDest, int yDest, bool verbal);
    bool CheckUnknownDirection(int xSource, int ySource, int xDest, int yDest, bool verbal);
    bool CheckBadMoveRowFull(int xSource, int ySource, int xDest, int yDest, bool verbal);
    void MoveLine(int xSource, int ySource, int xDest, int yDest);
    int CheckRowsToCapture();
    static std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckUnclearCaptures(std::vector<std::pair<std::vector<BoardCell*>, bool>>& final);
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesDownLeftUpRight();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesDownRightUpLeft();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> CheckCapturesLeftRight();

public:
    int whiteReserve;
    int blackReserve;

    Game(int boardSize, int pawnTakeThreshold, int whiteInitialPawns, int blackInitialPawns, int whiteReserve, int blackReserve, bool currentPlayer);
    explicit Game(Game* other);
    ~Game();


    void PrintBoard();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> MakeMove(std::vector<std::string>& arguments, bool verbal);
    void CaptureRow(const std::pair<std::vector<BoardCell*>, bool>& row);

    [[nodiscard]] int GetBoardSize() const;
    [[nodiscard]] std::list<BoardCell*>* GetBoard() const;
    [[nodiscard]] std::string GetGameState() const;
    void PrintGameState() const;
    [[nodiscard]] bool GetCurrentPlayer() const;

    std::vector<std::pair<std::vector<BoardCell*>, bool>> GetRowsToCapture();
};


#endif //GIPF_GAME_H
