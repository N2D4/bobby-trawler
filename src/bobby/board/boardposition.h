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

    constexpr BoardPosition(int column, int row) : column(column), row(row) { }

    constexpr bool operator==(const BoardPosition& w) const { return this->column == w.column && this->row == w.row; }
    constexpr bool operator!=(const BoardPosition& w) const { return !(*this == w); }

    constexpr bool isValid() const { return ((unsigned int) (this->column | this->row)) < 8; }
    inline float getPawnBonusScore(BoardSquare::Color color) const { return std::pow(std::max(0.0, color == BoardSquares::Colors::BLACK ? 3.0 - this->row : this->row - 4.0), 2) / 3; }
    
    // Implicit casting from/to C++ strings
    inline BoardPosition(const std::string& str) : BoardPosition(str[0] - 'a', str[1] - '1') { }
    inline BoardPosition& operator=(const std::string& str) const { return *this = BoardPosition(str); }
    inline operator std::string() const { return std::string({(char) ('a' + column),(char) ('1' + row), '\0'}); }

    // Implicit casting from C strings (for literals)
    constexpr BoardPosition(const char* str) : BoardPosition(str[0] - 'a', str[1] - '1') { }
    inline BoardPosition& operator=(const char* str) const { return *this = BoardPosition(str); }
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_