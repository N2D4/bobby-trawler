#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "boardsquares.h"

struct BoardPosition {
    int column;
    int row;

    BoardPosition(int column, int row);

    // Implicit casting from/to C++ strings
    BoardPosition(const std::string& str);
    BoardPosition& operator= (const std::string& str);
    operator std::string();

    // Implicit casting from C strings (for literals)
    BoardPosition(const char* str);
    BoardPosition& operator= (const char* str);
};

struct BoardMove {
    BoardPosition from;
    BoardPosition to;

    BoardMove(BoardPosition from, BoardPosition to);
};

class ChessBoard {
    BoardSquare squares[8][8];

    public: 
    ChessBoard();

    inline BoardSquare& operator[](BoardPosition position);
    bool isLegal(BoardMove move);

    std::string toHumanReadable(bool ansi=false);
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_