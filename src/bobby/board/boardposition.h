#ifndef BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_
#define BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "square/boardsquare.h"

/**
 * A simple structure for storing two values from 0-7, column and row, usually used to signal a position on the board,
 * whereas (column=0, row=0) is square a1. Strings in a form like a1, e4, g6 can implicitly be casted to a board
 * position, but that should only happen to parse user/GUI input as it is considerably less efficient than using the
 * (int, int) constructor.
 */
struct BoardPosition {
    int column;
    int row;

    BoardPosition(int column, int row);

    bool operator==(const BoardPosition& w);
    bool operator!=(const BoardPosition& w);

    bool isValid();
    float getPawnBonusScore(BoardSquare::Color color);
    
    // Implicit casting from/to C++ strings
    BoardPosition(const std::string& str);
    BoardPosition& operator= (const std::string& str);
    operator std::string();

    // Implicit casting from C strings (for literals)
    BoardPosition(const char* str);
    BoardPosition& operator= (const char* str);
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_