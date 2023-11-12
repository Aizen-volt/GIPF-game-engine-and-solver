#ifndef GIPF_GAME_ENGINE_AND_SOLVER_BOARD_H
#define GIPF_GAME_ENGINE_AND_SOLVER_BOARD_H

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include "BoardCell.h"

using std::vector;
using std::list;
using std::pair;

class Board {
private:
    // checks if the cell is a border cell
    bool cellIsBorder(int i, int j, int offset) const {
        return i == 0 || i == diagonalWithBorders - 1 ||
               j == 0 || j == diagonalWithBorders - 1 - offset;
    }


    // for displaying the board
    static void fillWithSpaces(int count) {
        for (int i = 0; i < count; i++) {
            std::cout << " ";
        }
    }


    // prints sign corresponding to the cell's state
    static void fillCell(BoardCell::State state) {
        switch (state) {
            case BoardCell::white:
                std::cout << "W";
                break;
            case BoardCell::black:
                std::cout << "B";
                break;
            case BoardCell::empty:
                std::cout << "_";
                break;
            case BoardCell::border:
                //std::cout << "+";
                break;
        }
    }


    // sets cells to border or empty
    void determineCellsStates() {
        for (int i = 0; i < diagonalWithBorders; i++) {
            // number of unused cells is equal to distance between the current row and the diagonal
            int offset = abs(i - diagonalWithBorders) / 2;

            for (int j = 0; j < diagonalWithBorders - offset; j++) {
                if (cellIsBorder(i, j, offset)) {
                    cells[i].emplace_back(Position(j, i), BoardCell::border);
                    continue;
                }
                cells[i].emplace_back(Position(j, i), BoardCell::empty);
                boardCellsCount++;
            }
        }
    }


    // returns positions of cells that are connected to the border cell
    vector<Position> findCellConnectionsBorder(const Position& source) {
        /*
             a5 b6 c7 d8 e9
            a4 b5 c6 d7 e8 f8
           a3 b4 c5 d6 e7 f7 g7
          a2 b3 c4 d5 e6 f6 g6 h6
         a1 b2 c3 d4 e5 f5 g5 h5 i5
          b1 c2 d3 e4 f4 g4 h4 i4
           c1 d2 e3 f3 g3 h3 i3
            d1 e2 f2 g2 h2 i2
             e1 f1 g1 h1 i1
        */

        int x = source.x;
        int y = source.y;

        vector<Position> connections;

        // a1 - i1
        if (x == 0) {
            // a1
            if (y == 0) {
                connections.emplace_back(x + 1, y + 1);
            }
            // e1
            else if (y == diagonalWithBorders / 2) {
                connections.emplace_back(x + 1, y);
            }
            // i1
            else if (y == diagonalWithBorders - 1) {
                connections.emplace_back(x + 1, y - 1);
            }
            // b1 - d1
            else if (y < diagonalWithBorders / 2) {
                connections.emplace_back(x + 1, y);
                connections.emplace_back(x + 1, y + 1);
            }
            // f1 - h1
            else {
                connections.emplace_back(x + 1, y - 1);
                connections.emplace_back(x + 1, y);
            }
        }

        // a5 - i5
        else if (x == cells[y].size() - 1) {
            // a5
            if (y == 0) {
                connections.emplace_back(x, y + 1);
            }
            // e9
            else if (y == diagonalWithBorders / 2) {
                connections.emplace_back(x - 1, y);
            }
            // i5
            else if (y == diagonalWithBorders - 1) {
                connections.emplace_back(x, y - 1);
            }
            // b6 - d8
            else if (y < diagonalWithBorders / 2) {
                connections.emplace_back(x - 1, y);
                connections.emplace_back(x, y + 1);
            }
            // f8 - h6
            else {
                connections.emplace_back(x, y - 1);
                connections.emplace_back(x - 1, y);
            }
        }

        // a2 - a4
        else if (y == 0) {
            connections.emplace_back(x, y + 1);
            connections.emplace_back(x + 1, y + 1);
        }

        // i2 - i4
        else {
            connections.emplace_back(x, y - 1);
            connections.emplace_back(x + 1, y - 1);
        }

        return connections;
    }


    // finds positions of cells that are connected to the normal cell
    void findCellConnectionsOther(const Position& source, multimap<Position, Position>& connections) const {
        /*
             a5 b6 c7 d8 e9
            a4 b5 c6 d7 e8 f8
           a3 b4 c5 d6 e7 f7 g7
          a2 b3 c4 d5 e6 f6 g6 h6
         a1 b2 c3 d4 e5 f5 g5 h5 i5
          b1 c2 d3 e4 f4 g4 h4 i4
           c1 d2 e3 f3 g3 h3 i3
            d1 e2 f2 g2 h2 i2
             e1 f1 g1 h1 i1
        */

        int x = source.x;
        int y = source.y;

        // up right, down left
        connections.insert({ Position(x + 1, y), Position(x - 1, y) });
        connections.insert({ Position(x - 1, y), Position(x + 1, y) });

        // up left, down right
        // e2-e8
        if (y == diagonalWithBorders / 2) {
            connections.insert({ Position(x, y - 1), Position(x - 1, y + 1) });
            connections.insert({ Position(x - 1, y + 1), Position(x, y - 1) });
        }
        // f2-f7, g2-g6, h2-h5
        else if (y > diagonalWithBorders / 2) {
            connections.insert({ Position(x + 1, y - 1), Position(x - 1, y + 1) });
            connections.insert({ Position(x - 1, y + 1), Position(x + 1, y - 1) });
        }
        // b2-b5, c2-c6, d2-d7
        else {
            connections.insert({ Position(x, y - 1), Position(x, y + 1) });
            connections.insert({ Position(x, y + 1), Position(x, y - 1) });
        }

        // left right
        // e2-e8
        if (y == diagonalWithBorders / 2) {
            connections.insert({ Position(x - 1, y - 1), Position(x, y + 1) });
            connections.insert({ Position(x, y + 1), Position(x - 1, y - 1) });
        }
        // f2-f7, g2-g6, h2-h5
        else if (y > diagonalWithBorders / 2) {
            connections.insert({ Position(x, y - 1), Position(x, y + 1) });
            connections.insert({ Position(x, y + 1), Position(x, y - 1) });
        }
        // b2-b5, c2-c6, d2-d7
        else {
            connections.insert({ Position(x - 1, y - 1), Position(x + 1, y + 1) });
            connections.insert({ Position(x + 1, y + 1), Position(x - 1, y - 1) });
        }
    }


    // finds positions of cells that are connected to the cells
    void findCellConnections() {
        for (int y = 0; y < diagonalWithBorders; y++) {
            for (int x = 0; x < cells[y].size(); x++) {
                auto cell = cells[y].begin();
                std::advance(cell, x);

                Position source = (*cell).position;

                if ((*cell).state == BoardCell::border) {
                    vector<Position> connections = findCellConnectionsBorder(source);
                    for (auto& connection : connections) {
                        (*cell).connections.insert({ source, connection });
                    }
                }

                findCellConnectionsOther(source, (*cell).connections);
            }
        }
    }

public:
    int size;
    int diagonal, diagonalWithBorders;
    int boardCellsCount = 0;

    vector<list<BoardCell>> cells;


    explicit Board(int size) : size(size) {
        diagonal = size * 2 - 1; // length of the longest row
        diagonalWithBorders = diagonal + 2;
        cells.resize(diagonalWithBorders);

        determineCellsStates();
        findCellConnections();
    }


    void print() const {
        /*
	        a5 b6 c7 d8 e9
		   a4 b5 c6 d7 e8 f8
		  a3 b4 c5 d6 e7 f7 g7
		 a2 b3 c4 d5 e6 f6 g6 h6
		a1 b2 c3 d4 e5 f5 g5 h5 i5
		 b1 c2 d3 e4 f4 g4 h4 i4
		  c1 d2 e3 f3 g3 h3 i3
		   d1 e2 f2 g2 h2 i2
            e1 f1 g1 h1 i1
	*/

        // first half
        for (int i = 0; i < size + 1; i++) {
            fillWithSpaces(size - 1);

            for (int j = 0; j < size + 1 + i; j++) {
                int offset = std::max(j - size, 0);

                auto cell = cells[j].end();
                std::advance(cell, offset - i - 1);

                fillCell((*cell).state);
            }
            std::cout << " ";
        }
        std::cout << "\n";

        // second half
        for (int i = 0; i < size; i++) {
            fillWithSpaces(i + 1);

            for (int j = 0; j < 2 * size - i; j++) {
                int offset = j >= size - i ? j - size + i + 1 : 0;

                auto cell = cells[i + j+ 1].begin();
                if (i != size - 1) {
                    std::advance(cell, j - offset);
                }
                fillCell((*cell).state);
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
};

#endif //GIPF_GAME_ENGINE_AND_SOLVER_BOARD_H
