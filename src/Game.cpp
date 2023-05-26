#include <iostream>
#include <string>
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

    if (!CheckGameConfigCorrectness()) {
        std::cout << "Game config does not meet requirements!\n";
        exit(1);
    }
    InitBoardArray();
    bool inputCorrect = FillBoard();

    if (!inputCorrect)
        std::cout << "WRONG_BOARD_ROW_LENGTH\n";
    else if (whiteOnBoard + whiteReserve != whiteInitialPawns)
        std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
    else if (blackOnBoard + blackReserve != blackInitialPawns)
        std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
    else
        std::cout << "BOARD_STATE_OK\n";
}

Game::~Game() {
    DeleteBoardArray();
}

bool Game::CheckGameConfigCorrectness() const {
    if (boardSize < 2)
        return false;
    if (pawnTakeThreshold < 2 || pawnTakeThreshold >= 2 * boardSize - 1)
        return false;
    if (whiteInitialPawns <= 3 || blackInitialPawns <= 3)
        return false;
    return true;
}

void Game::InitBoardArray() {
    board = new std::list<BoardCell*>[DIAGONAL_LENGTH + 2];
    //cells unreachable for pawns are marked nullptr
    for (int i = 0; i < DIAGONAL_LENGTH + 2; i++) {
        //number of unused cells is equal to distance between current row and diagonal
        int offset = abs(i - (DIAGONAL_LENGTH + 2) / 2);
        for (int j = 0; j < DIAGONAL_LENGTH + 2 - offset; j++) {
            if ( i == 0 || i == DIAGONAL_LENGTH + 1 || j == 0 || j == DIAGONAL_LENGTH + 2 - offset - 1) {
                board[i].push_back(new BoardCell(i, j, BORDER));
                continue;
            }
            board[i].push_back(new BoardCell(i, j, EMPTY));
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
        if (!temp.empty()) {
            inputCorrect = false;
        }
    }
    return inputCorrect;
}

void Game::PrintBoard() {
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
                    std::cout << "+";
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
                    std::cout << "+";
                    break;
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}