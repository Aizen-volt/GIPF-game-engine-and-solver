#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <optional>
#include "Game.h"


Game::Game(int boardSize, int pawnTakeThreshold, int whiteInitialPawns, int blackInitialPawns, int whiteReserve, int blackReserve, bool currentPlayer) {
    this->boardSize = boardSize;
    this->pawnTakeThreshold = pawnTakeThreshold;
    this->whiteInitialPawns = whiteInitialPawns;
    this->blackInitialPawns = blackInitialPawns;
    this->whiteReserve = whiteReserve;
    this->blackReserve = blackReserve;
    this->currentPlayer = currentPlayer;

    whiteOnBoard = 0;
    blackOnBoard = 0;
    boardCellsCount = 0;

    InitBoardArray();
    bool inputCorrect = FillBoard();
    FindCellConnections();

    int rowsToCapture;

    if (!inputCorrect)
        std::cout << "WRONG_BOARD_ROW_LENGTH\n";
    else if (whiteOnBoard + whiteReserve > whiteInitialPawns) {
        std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
        whiteOnBoard = blackOnBoard = -1;
    }
    else if (blackOnBoard + blackReserve > blackInitialPawns) {
        std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
        whiteOnBoard = blackOnBoard = -1;
    }
    else {
        rowsToCapture = CheckRowsToCapture();
        if (rowsToCapture > 0)
            std::cout << "ERROR_FOUND_" << rowsToCapture << "_ROW" << (rowsToCapture > 1 ? "S" : "") << "_OF_LENGTH_K\n";
        else
            std::cout << "BOARD_STATE_OK\n";
    }

    if (currentPlayer == BLACK && blackReserve <= 0)
        gameState = "white_win";
    else if (currentPlayer == WHITE && whiteReserve <= 0)
        gameState = "black_win";
    else if (blackOnBoard + whiteOnBoard >= boardCellsCount)
        gameState = (currentPlayer == BLACK ? "dead_lock black" : "dead_lock white");
    else
        gameState = "in_progress";
}


Game::Game(Game* other) {
    this->boardSize = other->boardSize;
    this->pawnTakeThreshold = other->pawnTakeThreshold;
    this->whiteInitialPawns = other->whiteInitialPawns;
    this->blackInitialPawns = other->blackInitialPawns;
    this->whiteReserve = other->whiteReserve;
    this->blackReserve = other->blackReserve;
    this->currentPlayer = other->currentPlayer;
    this->whiteOnBoard = other->whiteOnBoard;
    this->blackOnBoard = other->blackOnBoard;
    this->gameState = other->gameState;
    this->boardCellsCount = other->boardCellsCount;

    board = new std::list<BoardCell*>[DIAGONAL_LENGTH + 2];
    for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
        for (auto it = other->board[i].begin(); it != other->board[i].end(); it++) {
            board[i].push_back(new BoardCell((*it)->x, (*it)->y, (*it)->GetState()));
        }
    }

    FindCellConnections();
}


Game::~Game() {
    DeleteBoardArray();
}



std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::MakeMove(std::vector<std::string>& arguments, bool verbal) {
    int xSource, ySource, xDest, yDest;
    std::string move = arguments[0];
    DetermineMoveCoords(move, &xSource, &ySource, &xDest, &yDest);

    if (CheckBadIndex(xSource, ySource, verbal) ||
        CheckBadIndex(xDest, yDest, verbal) ||
        CheckBadMoveWrongField(xSource, ySource, xDest, yDest, verbal) ||
        CheckUnknownDirection(xSource, ySource, xDest, yDest, verbal) ||
        CheckBadMoveRowFull(xSource, ySource, xDest, yDest, verbal)) {
        gameState = "bad_move ";
        gameState += currentPlayer == BLACK ? "black" : "white" + arguments[0];
        return {};
    }


    MoveLine(xSource, ySource, xDest, yDest);


    std::vector<std::pair<std::vector<BoardCell*>, bool>> rowsToCapture = GetRowsToCapture();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> unclearRows = CheckUnclearCaptures(rowsToCapture);

    if (!verbal)
        return unclearRows;

    if (!unclearRows.empty()) {
        bool color = arguments[1] == "b:";
        int y1 = arguments[2][0] - 'a';
        int x1 = arguments[2][1] - '0' - 1;
        int y2 = arguments[3][0] - 'a';
        int x2 = arguments[3][1] - '0' - 1;

        for (auto &row : unclearRows) {
            bool foundFirst = false;
            bool foundSecond = false;
            int counter = 0;

            for (auto *current : row.first) {
                if (current->x == x1 && current->y == y1)
                    foundFirst = true;
                if (current->x == x2 && current->y == y2)
                    foundSecond = true;

                if (foundFirst || foundSecond)
                    counter++;

                if (foundFirst && foundSecond) {
                    if (counter < pawnTakeThreshold) {
                        gameState = "bad_move ";
                        gameState += currentPlayer == BLACK ? "black" : "white" + arguments[0];
                        if (verbal)
                            std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
                        return {};
                    }

                    if (current->GetState() != color) {
                        gameState = "bad_move ";
                        gameState += currentPlayer == BLACK ? "black" : "white" + arguments[0];
                        if (verbal)
                            std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
                        return {};
                    }
                    if (verbal)
                        std::cout << "MOVE_COMMITTED\n";
                    for (auto *current : row.first) {
                        if (current->GetState() == row.second) {
                            row.second == BLACK ? blackReserve++ : whiteReserve++;
                        }
                        current->GetState() == BLACK ? blackOnBoard-- : whiteOnBoard--;
                        current->SetState(EMPTY);
                    }
                    return {};
                }
            }
        }
        gameState = "bad_move ";
        gameState += currentPlayer == BLACK ? "black" : "white" + arguments[0];
        if (verbal)
            std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
    }
    else if (!rowsToCapture.empty()) {
        for (const auto& row : rowsToCapture) {
            for (auto *cell: row.first) {
                if (cell->GetState() == row.second) {
                    row.second == BLACK ? blackReserve++ : whiteReserve++;
                }
                cell->GetState() == BLACK ? blackOnBoard-- : whiteOnBoard--;
                cell->SetState(EMPTY);
            }
        }
        if (verbal)
            std::cout << "MOVE_COMMITTED\n";
    }
    else
        if (verbal)
            std::cout << "MOVE_COMMITTED\n";

        return {};
}


std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::GetRowsToCapture() {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> first = CheckCapturesLeftRight();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> second = CheckCapturesDownRightUpLeft();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> third = CheckCapturesDownLeftUpRight();
    std::vector<std::pair<std::vector<BoardCell*>, bool>> final;

    std::copy(first.begin(), first.end(), std::back_inserter(final));
    std::copy(second.begin(), second.end(), std::back_inserter(final));
    std::copy(third.begin(), third.end(), std::back_inserter(final));

    return final;
}


void Game::MoveLine(int xSource, int ySource, int xDest, int yDest) {
    int xCurrent = xDest, yCurrent = yDest, xPrev = xSource, yPrev = ySource;

    //stores info about pawns that will be moved and their destination points
    std::stack<std::pair<BoardCell*, std::pair<int, int>>> pawnsToMove;

    auto it = board[yCurrent].begin();
    std::advance(it, xCurrent);

    while ((*it)->GetState() != EMPTY && (*it)->GetState() != BORDER) {
        std::pair<int, int> next = (*it)->foundConnections.find({xPrev, yPrev})->second;
        xPrev = xCurrent;
        yPrev = yCurrent;
        xCurrent = next.first;
        yCurrent = next.second;
        pawnsToMove.emplace(*it, next);
        it = board[yCurrent].begin();
        std::advance(it, xCurrent);
    }
    while (!pawnsToMove.empty()) {
        std::pair<BoardCell*, std::pair<int, int>> temp = pawnsToMove.top();
        pawnsToMove.pop();
        BoardCell* cell = temp.first;
        std::pair<int, int> dest = temp.second;
        auto destCell = board[dest.second].begin();
        std::advance(destCell, dest.first);
        (*destCell)->SetState(cell->GetState());
        cell->SetState(EMPTY);
    }
    it = board[yDest].begin();
    std::advance(it, xDest);
    (*it)->SetState(currentPlayer);

    currentPlayer == BLACK ? blackReserve-- : whiteReserve--;
    currentPlayer == BLACK ? blackOnBoard++ : whiteOnBoard++;

    currentPlayer = !currentPlayer;

    if (currentPlayer == BLACK && blackReserve <= 0)
        gameState = "white_win";
    else if (currentPlayer == WHITE && whiteReserve <= 0)
        gameState = "black_win";
    else if (blackOnBoard + whiteOnBoard >= boardCellsCount)
        gameState = (currentPlayer == BLACK ? "dead_lock black" : "dead_lock white");
}


std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::CheckUnclearCaptures(std::vector<std::pair<std::vector<BoardCell*>, bool>>& final) {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> answer;

    for (int i = 0; i < final.size(); i++) {
        for (int j = i + 1; j < final.size(); j++) {
            for (int k = 0; k < final[i].first.size(); k++) {
                for (int l = 0; l < final[j].first.size(); l++) {
                    if (final[i].first[k]->y == final[j].first[l]->y && final[i].first[k]->x == final[j].first[l]->x) {
                        answer.push_back(final[i]);
                        answer.push_back(final[j]);
                    }
                }
            }
        }
    }

    return answer;
}


bool Game::CheckBadMoveRowFull(int xSource, int ySource, int xDest, int yDest, bool verbal) {
    int xCurrent = xDest, yCurrent = yDest, xPrev = xSource, yPrev = ySource;

    auto it = board[yCurrent].begin();
    std::advance(it, xCurrent);

    while ((*it)->GetState() != BORDER) {
        if ((*it)->GetState() == EMPTY)
            return false;
        std::pair<int, int> next = (*it)->foundConnections.find({xPrev, yPrev})->second;
        xPrev = xCurrent;
        yPrev = yCurrent;
        xCurrent = next.first;
        yCurrent = next.second;
        it = board[yCurrent].begin();
        std::advance(it, xCurrent);
    }
    if (verbal)
        std::cout << "BAD_MOVE_ROW_IS_FULL\n";
    return true;
}


bool Game::CheckUnknownDirection(int xSource, int ySource, int xDest, int yDest, bool verbal) {
    auto it = board[ySource].begin();
    std::advance(it, xSource);
    std::pair<int, int> dest = {xDest, yDest};
    for (auto & foundConnection : (*it)->foundConnections) {
        if (foundConnection.second == dest)
            return false;
    }
    if (verbal)
        std::cout << "UNKNOWN_MOVE_DIRECTION\n";
    return true;
}


void Game::DetermineMoveCoords(std::string& move, int* xSource, int* ySource, int* xDest, int* yDest) {
    *ySource = move[0] - 'a';
    move.erase(move.begin());
    std::string temp;
    while (move[0] != '-') {
        temp.push_back(move[0]);
        move.erase(move.begin());
    }
    *xSource = std::stoi(temp) - 1;
    move.erase(move.begin());
    *yDest = move[0] - 'a';
    move.erase(move.begin());
    *xDest = std::stoi(move) - 1;
}


bool Game::CheckBadIndex(int x, int y, bool verbal) {
    if (y < 0 || y >= DIAGONAL_LENGTH + 2 || x < 0 || x >= board[y].size()) {
        if (verbal)
            std::cout << "BAD_MOVE_" << char(y + 'a') << x + 1 << "_IS_WRONG_INDEX\n";
        return true;
    }
    return false;
}


bool Game::CheckBadMoveWrongField(int xSource, int ySource, int xDest, int yDest, bool verbal) {
    auto it = board[ySource].begin();
    std::advance(it, xSource);
    if ((*it)->GetState() != BORDER) {
        if (verbal)
            std::cout << "BAD_MOVE_" << char(ySource + 'a') << xSource + 1 << "_IS_WRONG_STARTING_FIELD\n";
        return true;
    }
    it = board[yDest].begin();
    std::advance(it, xDest);
    if ((*it)->GetState() == BORDER) {
        if (verbal)
            std::cout << "BAD_MOVE_" << char(yDest + 'a') << xDest + 1 << "_IS_WRONG_DESTINATION_FIELD\n";
        return true;
    }
    return false;
}




std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::CheckCapturesDownLeftUpRight() {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> output;

    for (int i = 1; i <= DIAGONAL_LENGTH; i++) {
        auto cell = board[i].begin();
        std::advance(cell, 1);

        int state = (*cell)->GetState();
        int counter = 0;
        std::vector<BoardCell*> line;

        while (true) {
            if ((*cell)->GetState() == BORDER)
                break;

            if ((*cell)->GetState() == EMPTY) {
                if (counter >= pawnTakeThreshold) {
                    int whiteCount = 0;
                    int blackCount = 0;
                    for (BoardCell* b : line) {
                        b->GetState() == WHITE ? whiteCount++ : blackCount++;
                        if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                            output.emplace_back(line, blackCount > whiteCount);
                            break;
                        }
                    }
                }
                line.clear();
                state = EMPTY;
                counter = 1;
            }
            else {
                if ((*cell)->GetState() == state) {
                    counter++;
                    line.push_back(*cell);
                }
                else {
                    state = (*cell)->GetState();
                    if (counter < pawnTakeThreshold)
                        counter = 1;
                    line.push_back(*cell);
                }
            }
            std::advance(cell, 1);
        }

        if (counter >= pawnTakeThreshold) {
            int whiteCount = 0;
            int blackCount = 0;
            for (BoardCell* b : line) {
                b->GetState() == WHITE ? whiteCount++ : blackCount++;
                if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                    output.emplace_back(line, blackCount > whiteCount);
                    break;
                }
            }
        }
        line.clear();
    }
    return output;
}


std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::CheckCapturesDownRightUpLeft() {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> output;

    for (int i = 1; i <= DIAGONAL_LENGTH; i++) {
        std::list<BoardCell*>::iterator it;
        int counter = 0;
        int xCurr, yCurr, xPrev, yPrev;

        if (i <= boardSize) {
            yCurr = 1;
            xCurr = i;
            it = board[yCurr].begin();
            std::advance(it, xCurr);
        }
        else {
            yCurr = 1 + i - boardSize;
            xCurr = board[yCurr].size() - 2;
            it = board[yCurr].end();
            std::advance(it, -2);
        }

        yPrev = yCurr - 1;
        xPrev = xCurr;
        int state = (*it)->GetState();

        std::vector<BoardCell*> line;

        while (true) {
            if ((*it)->GetState() == BORDER)
                break;

            if ((*it)->GetState() == EMPTY) {
                if (counter >= pawnTakeThreshold) {
                    int whiteCount = 0;
                    int blackCount = 0;
                    for (BoardCell* b : line) {
                        b->GetState() == WHITE ? whiteCount++ : blackCount++;
                        if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                            output.emplace_back(line, blackCount > whiteCount);
                            break;
                        }
                    }
                }
                line.clear();
                state = EMPTY;
                counter = 1;
            }
            else {
                if ((*it)->GetState() == state) {
                    counter++;
                    line.push_back(*it);
                }
                else {
                    state = (*it)->GetState();
                    if (counter < pawnTakeThreshold)
                        counter = 1;
                    line.push_back(*it);
                }
            }
            //std::cout << char(yCurr + 'a') << xCurr + 1 << " ";

            std::pair<int, int> next = (*it)->foundConnections.find({xPrev, yPrev})->second;
            xPrev = xCurr;
            yPrev = yCurr;
            xCurr = next.first;
            yCurr = next.second;
            it = board[yCurr].begin();
            std::advance(it, xCurr);
        }

        if (counter >= pawnTakeThreshold) {
            int whiteCount = 0;
            int blackCount = 0;
            for (BoardCell* b : line) {
                b->GetState() == WHITE ? whiteCount++ : blackCount++;
                if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                    output.emplace_back(line, blackCount > whiteCount);
                    break;
                }
            }
        }
        line.clear();
    }
    return output;
}


std::vector<std::pair<std::vector<BoardCell*>, bool>> Game::CheckCapturesLeftRight() {
    std::vector<std::pair<std::vector<BoardCell*>, bool>> output;

    for (int i = 1; i <= boardSize; i++) {
        auto it = board[1].begin();
        std::advance(it, i);

        int counter = 0;
        int xCurr = i, yCurr = 1, xPrev = xCurr - 1, yPrev = yCurr - 1;
        int state = (*it)->GetState();

        std::vector<BoardCell*> line;

        while (true) {
            if ((*it)->GetState() == BORDER)
                break;

            if ((*it)->GetState() == EMPTY) {
                if (counter >= pawnTakeThreshold) {
                    int whiteCount = 0;
                    int blackCount = 0;
                    for (BoardCell* b : line) {
                        b->GetState() == WHITE ? whiteCount++ : blackCount++;
                        if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                            output.emplace_back(line, blackCount > whiteCount);
                            break;
                        }
                    }
                }
                line.clear();
                state = EMPTY;
                counter = 1;
            }
            else {
                if ((*it)->GetState() == state) {
                    counter++;
                    line.push_back(*it);
                }
                else {
                    state = (*it)->GetState();
                    if (counter < pawnTakeThreshold)
                        counter = 1;
                    line.push_back(*it);
                }
            }

            std::pair<int, int> next = (*it)->foundConnections.find({xPrev, yPrev})->second;
            xPrev = xCurr;
            yPrev = yCurr;
            xCurr = next.first;
            yCurr = next.second;
            it = board[yCurr].begin();
            std::advance(it, xCurr);
        }

        if (counter >= pawnTakeThreshold) {
            int whiteCount = 0;
            int blackCount = 0;
            for (BoardCell* b : line) {
                b->GetState() == WHITE ? whiteCount++ : blackCount++;
                if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                    output.emplace_back(line, blackCount > whiteCount);
                    break;
                }
            }
        }
        line.clear();
    }

    for (int i = 1; i < boardSize; i++) {
        auto it = board[1 + i].begin();
        std::advance(it, 1);

        int counter = 0;
        int xCurr = 1, yCurr = 1 + i, xPrev = xCurr - 1, yPrev = yCurr - 1;
        int state = (*it)->GetState();

        std::vector<BoardCell*> line;

        while (true) {
            if ((*it)->GetState() == BORDER)
                break;

            if ((*it)->GetState() == EMPTY) {
                if (counter >= pawnTakeThreshold) {
                    int whiteCount = 0;
                    int blackCount = 0;
                    for (BoardCell* b : line) {
                        b->GetState() == WHITE ? whiteCount++ : blackCount++;
                        if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                            output.emplace_back(line, blackCount > whiteCount);
                            break;
                        }
                    }
                }
                line.clear();
                state = EMPTY;
                counter = 1;
            }
            else {
                if ((*it)->GetState() == state) {
                    counter++;
                    line.push_back(*it);
                }
                else {
                    state = (*it)->GetState();
                    if (counter < pawnTakeThreshold)
                        counter = 1;
                    line.push_back(*it);
                }
            }

            std::pair<int, int> next = (*it)->foundConnections.find({xPrev, yPrev})->second;
            xPrev = xCurr;
            yPrev = yCurr;
            xCurr = next.first;
            yCurr = next.second;
            it = board[yCurr].begin();
            std::advance(it, xCurr);
        }

        if (counter >= pawnTakeThreshold) {
            int whiteCount = 0;
            int blackCount = 0;
            for (BoardCell* b : line) {
                b->GetState() == WHITE ? whiteCount++ : blackCount++;
                if (whiteCount >= pawnTakeThreshold || blackCount >= pawnTakeThreshold) {
                    output.emplace_back(line, blackCount > whiteCount);
                    break;
                }
            }
        }
        line.clear();
    }
    return output;
}


int Game::CheckRowsToCapture() {
    int captureRowsCount = 0;
    captureRowsCount += CheckCapturesDownLeftUpRight().size();
    captureRowsCount += CheckCapturesDownRightUpLeft().size();
    captureRowsCount += CheckCapturesLeftRight().size();

    return captureRowsCount;
}


int Game::GetBoardSize() const {
    return boardSize;
}


std::list<BoardCell *> *Game::GetBoard() const {
    return board;
}


std::string Game::GetGameState() const {
    return gameState;
}


void Game::PrintGameState() const {
    if (gameState == "in_progress")
        std::cout << "GAME_IN_PROGRESS\n";
    else if (gameState == "black_win" || gameState == "dead_lock white")
        std::cout << "THE_WINNER_IS_BLACK\n";
    else
        std::cout << "THE_WINNER_IS_WHITE\n";
}


bool Game::GetCurrentPlayer() const {
    return currentPlayer;
}


void Game::CaptureRow(const std::pair<std::vector<BoardCell *>, bool>& row) {
    for (auto *current : row.first) {
        for (int y = 0; y < DIAGONAL_LENGTH + 2; y++) {
            for (auto & it : board[y]) {
                if (it->x == current->x && it->y == current->y) {
                    if (it->GetState() == row.second) {
                        row.second == BLACK ? blackReserve++ : whiteReserve++;
                    }
                    it->GetState() == BLACK ? blackOnBoard-- : whiteOnBoard--;
                    it->SetState(EMPTY);
                }
            }
        }
    }
}
