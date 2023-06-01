#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
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

}


Game::~Game() {
    DeleteBoardArray();
}


void Game::InitBoardArray() {
    board = new std::list<BoardCell*>[DIAGONAL_LENGTH + 2];
    //cells unreachable for pawns are marked nullptr
    for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
        //number of unused cells is equal to distance between current row and diagonal
        int offset = abs(i - (DIAGONAL_LENGTH + 2) / 2);
        for (int j = 0; j < DIAGONAL_LENGTH + 2 - offset; j++) {
            if ( i == 0 || i == DIAGONAL_LENGTH + 1 || j == 0 || j == DIAGONAL_LENGTH + 2 - offset - 1) {
                board[i].push_back(new BoardCell(j, i, BORDER));
                continue;
            }
            board[i].push_back(new BoardCell(j, i, EMPTY));
        }
    }
}


void Game::DeleteBoardArray() {
    delete[] board;
}


bool Game::FillBoard() {
    bool inputCorrect = true;
    for (int i = 0; i < boardSize + 1; i++) {
        if (!inputCorrect) {
            std::string temp;
            getline(std::cin, temp);
            continue;
        }
        for (int j = 0; j < boardSize + 1 + i; j++) {
            auto it = board[j].end();
            int offset = 0;
            if (j >= boardSize + 1)
                offset = j - boardSize;
            std::advance(it, -1 - i + offset);
            if ((*it)->GetState() != BORDER) {
                char state;
                std::cin >> state;
                if (state == '_')
                    (*it)->SetState(EMPTY);
                else if (state == 'W') {
                    (*it)->SetState(WHITE);
                    whiteOnBoard++;
                }
                else if (state == 'B') {
                    (*it)->SetState(BLACK);
                    blackOnBoard++;
                }
            }
        }
        std::string temp;
        getline(std::cin, temp);
        temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
        if (!temp.empty()) {
            inputCorrect = false;
        }
    }
    for (int i = 0; i < boardSize - 1; i++) {
        if (!inputCorrect) {
            std::string temp;
            getline(std::cin, temp);
            continue;
        }
        for (int j = 0; j < DIAGONAL_LENGTH + 1 - i; j++) {
            auto it = board[i + j + 1].begin();
            int offset = 0;
            if (j >= boardSize - i)
                offset = j - (boardSize - i - 1);
            if (i != boardSize - 1) {
                std::advance(it, j - offset);
            }
            if ((*it)->GetState() != BORDER) {
                char state;
                std::cin >> state;
                if (state == '_')
                    (*it)->SetState(EMPTY);
                else if (state == 'W') {
                    (*it)->SetState(WHITE);
                    whiteOnBoard++;
                }
                else if (state == 'B') {
                    (*it)->SetState(BLACK);
                    blackOnBoard++;
                }
            }
        }
        std::string temp;
        getline(std::cin, temp);
        temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
        if (!temp.empty()) {
            inputCorrect = false;
        }
    }
    if (!inputCorrect)
        whiteOnBoard = blackOnBoard = -1;
    return inputCorrect;
}


void Game::PrintBoard() {
    if (blackOnBoard + whiteOnBoard == -2) {
        std::cout << "EMPTY_BOARD\n";
        return;
    }

    std:: cout << boardSize << " " << pawnTakeThreshold << " " << whiteInitialPawns << " " << blackInitialPawns << "\n";
    std::cout << whiteReserve << " " << blackReserve << " " << (currentPlayer ? "B" : "W");

    for (int i = 0; i < boardSize + 1; i++) {
        for (int j = 0; j < DIAGONAL_LENGTH + 1 - boardSize - i; j++) {
            std::cout << " ";
        }
        for (int j = 0; j < boardSize + 1 + i; j++) {
            auto it = board[j].end();
            int offset = 0;
            if (j >= boardSize + 1)
                offset = j - boardSize;
            std::advance(it, -1 - i + offset);
            switch ((*it)->GetState()) {
                case EMPTY:
                    std::cout << "_";
                    break;
                case WHITE:
                    std::cout << "W";
                    break;
                case BLACK:
                    std::cout << "B";
                    break;
                case BORDER:
                    //std::cout << "+";
                    break;
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < i + 1; j++) {
            std::cout << " ";
        }
        for (int j = 0; j < DIAGONAL_LENGTH + 1 - i; j++) {
            auto it = board[i + j + 1].begin();
            int offset = 0;
            if (j >= boardSize - i)
                offset = j - (boardSize - i - 1);
            if (i != boardSize - 1) {
                std::advance(it, j - offset);
            }
            switch ((*it)->GetState()) {
                case EMPTY:
                    std::cout << "_";
                    break;
                case WHITE:
                    std::cout << "W";
                    break;
                case BLACK:
                    std::cout << "B";
                    break;
                case BORDER:
                    //std::cout << "+";
                    break;
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}


void Game::MakeMove(std::vector<std::string>& arguments) {
    int xSource, ySource, xDest, yDest;
    std::string move = arguments[0];
    DetermineMoveCoords(move, &xSource, &ySource, &xDest, &yDest);
    if (CheckBadIndex(xSource, ySource) || CheckBadIndex(xDest, yDest))
        return;
    if (CheckBadMoveWrongField(xSource, ySource, xDest, yDest))
        return;
    if (CheckUnknownDirection(xSource, ySource, xDest, yDest))
        return;
    if (CheckBadMoveRowFull(xSource, ySource, xDest, yDest))
        return;

    MoveLine(xSource, ySource, xDest, yDest);
    std::vector<std::pair<std::vector<BoardCell*>, bool>> rowsToCapture = GetRowsToCapture();
    if (!rowsToCapture.empty() && arguments.size() == 4) {
        bool color = arguments[1] == "b:";
        int y1 = arguments[2][0] - 'a';
        int x1 = arguments[2][1] - '0' - 1;
        int y2 = arguments[3][0] - 'a';
        int x2 = arguments[3][1] - '0' - 1;

        for (auto &row : rowsToCapture) {
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
                        std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
                        return;
                    }

                    if (current->GetState() != color) {
                        std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
                        return;
                    }

                    std::cout << "MOVE_COMMITTED\n";
                    for (auto *current : row.first) {
                        if (current->GetState() == row.second) {
                            row.second == BLACK ? blackReserve++ : whiteReserve++;
                        }
                        current->GetState() == BLACK ? blackOnBoard-- : whiteOnBoard--;
                        current->SetState(EMPTY);
                    }
                    return;
                }
            }
        }
        std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
    }
    else if (!rowsToCapture.empty()) {
        for (const auto& row : rowsToCapture)
        for (auto* cell : row.first) {
            if (cell->GetState() == row.second) {
                row.second == BLACK ? blackReserve++ : whiteReserve++;
            }
            cell->GetState() == BLACK ? blackOnBoard-- : whiteOnBoard--;
            cell->SetState(EMPTY);
        }
        std::cout << "MOVE_COMMITTED\n";
    }
    else
        std::cout << "MOVE_COMMITTED\n";
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

    currentPlayer ? blackReserve-- : whiteReserve--;
    currentPlayer ? blackOnBoard++ : whiteOnBoard++;

    currentPlayer = !currentPlayer;
}


bool Game::CheckBadMoveRowFull(int xSource, int ySource, int xDest, int yDest) {
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
    std::cout << "BAD_MOVE_ROW_IS_FULL\n";
    return true;
}


bool Game::CheckUnknownDirection(int xSource, int ySource, int xDest, int yDest) {
    auto it = board[ySource].begin();
    std::advance(it, xSource);
    std::pair<int, int> dest = {xDest, yDest};
    for (auto & foundConnection : (*it)->foundConnections) {
        if (foundConnection.second == dest)
            return false;
    }
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


bool Game::CheckBadIndex(int x, int y) {
    if (y < 0 || y >= DIAGONAL_LENGTH + 2 || x < 0 || x >= board[y].size()) {
        std::cout << "BAD_MOVE_" << char(y + 'a') << x + 1 << "_IS_WRONG_INDEX\n";
        return true;
    }
    return false;
}


bool Game::CheckBadMoveWrongField(int xSource, int ySource, int xDest, int yDest) {
    auto it = board[ySource].begin();
    std::advance(it, xSource);
    if ((*it)->GetState() != BORDER) {
        std::cout << "BAD_MOVE_" << char(ySource + 'a') << xSource + 1 << "_IS_WRONG_STARTING_FIELD\n";
        return true;
    }
    it = board[yDest].begin();
    std::advance(it, xDest);
    if ((*it)->GetState() == BORDER) {
        std::cout << "BAD_MOVE_" << char(yDest + 'a') << xDest + 1 << "_IS_WRONG_DESTINATION_FIELD\n";
        return true;
    }
    return false;
}


void Game::FindCellConnections() {
    for (int y = 0; y < DIAGONAL_LENGTH + 2; y++) {
        for (int x = 0; x < board[y].size(); x++) {
            auto it = board[y].begin();
            std::advance(it, x);

            std::pair<int, int> source;
            std::pair<int, int> dest;

            //borders
            if ((*it)->GetState() == BORDER) {
                if (x == 0) {
                    if (y == 0) {
                        source = {x, y};
                        dest = {x + 1, y + 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y == (DIAGONAL_LENGTH + 2) / 2) {
                        source = {x, y};
                        dest = {x + 1, y};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y == DIAGONAL_LENGTH + 1) {
                        source = {x, y};
                        dest = {x + 1, y - 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y < (DIAGONAL_LENGTH + 2) / 2) {
                        source = {x, y};
                        dest = {x + 1, y};
                        (*it)->foundConnections.insert({source, dest});

                        source = {x, y};
                        dest = {x + 1, y + 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else {
                        source = {x, y};
                        dest = {x + 1, y - 1};
                        (*it)->foundConnections.insert({source, dest});

                        source = {x, y};
                        dest = {x + 1, y};
                        (*it)->foundConnections.insert({source, dest});
                    }
                }
                else if (x == board[y].size() - 1) {
                    if (y == 0) {
                        source = {x, y};
                        dest = {x, y + 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y == (DIAGONAL_LENGTH + 2) / 2) {
                        source = {x, y};
                        dest = {x - 1, y};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y == DIAGONAL_LENGTH + 1) {
                        source = {x, y};
                        dest = {x, y - 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else if (y < (DIAGONAL_LENGTH + 2) / 2) {
                        source = {x, y};
                        dest = {x - 1 , y};
                        (*it)->foundConnections.insert({source, dest});

                        source = {x, y};
                        dest = {x, y + 1};
                        (*it)->foundConnections.insert({source, dest});
                    }
                    else {
                        source = {x, y};
                        dest = {x, y - 1};
                        (*it)->foundConnections.insert({source, dest});

                        source = {x, y};
                        dest = {x - 1, y};
                        (*it)->foundConnections.insert({source, dest});
                    }
                }
                else if (y == 0) {
                    source = {x, y};
                    dest = {x, y + 1};
                    (*it)->foundConnections.insert({source, dest});

                    source = {x, y};
                    dest = {x + 1, y + 1};
                    (*it)->foundConnections.insert({source, dest});
                }
                else {
                    source = {x, y};
                    dest = {x, y - 1};
                    (*it)->foundConnections.insert({source, dest});

                    source = {x, y};
                    dest = {x + 1, y - 1};
                    (*it)->foundConnections.insert({source, dest});
                }
                continue;
            }

            //up-right down-left
            source = {x + 1, y};
            dest = {x - 1, y};
            (*it)->foundConnections.insert({source, dest});

            source = {x - 1, y};
            dest = {x + 1, y};
            (*it)->foundConnections.insert({source, dest});

            //left right
            if (y == (DIAGONAL_LENGTH + 2) / 2) {
                source = {x - 1, y - 1};
                dest = {x, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x, y + 1};
                dest = {x - 1, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }
            else if (y > (DIAGONAL_LENGTH + 2) / 2) {
                source = {x, y - 1};
                dest = {x, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x, y + 1};
                dest = {x, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }
            else {
                source = {x - 1, y - 1};
                dest = {x + 1, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x + 1, y + 1};
                dest = {x - 1, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }

            //up-left down-right
            if (y == (DIAGONAL_LENGTH + 2) / 2) {
                source = {x, y - 1};
                dest = {x - 1, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x - 1, y + 1};
                dest = {x, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }
            else if (y > (DIAGONAL_LENGTH + 2) / 2) {
                source = {x + 1, y - 1};
                dest = {x - 1, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x - 1, y + 1};
                dest = {x + 1, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }
            else {
                source = {x, y - 1};
                dest = {x, y + 1};
                (*it)->foundConnections.insert({source, dest});

                source = {x, y + 1};
                dest = {x, y - 1};
                (*it)->foundConnections.insert({source, dest});
            }
        }
    }
//    for (int y = 0; y < DIAGONAL_LENGTH + 2; y++) {
//        for (int x = 0; x < board[y].size(); x++) {
//            auto it = board[y].begin();
//            std::advance(it, x);
//            std::cout << char(y + 'a') << x + 1 << " ";
//            for (auto const& pair : (*it)->foundConnections) {
//                std::cout << char(pair.first.second + 'a') << pair.first.first + 1 << " -> " << char(pair.second.second + 'a') << pair.second.first + 1 << "  ";
//            }
//            std::cout << "\n";
//        }
//    }
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