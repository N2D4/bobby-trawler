#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "boardsquares.h"

struct BoardPosition {
    int column;
    int row;
    std::string toString();
};

struct ChessBoard {
    BoardSquare squares[8][8];
    ChessBoard();
    BoardSquare getSquare(BoardPosition position);
};

struct ChessMove {
    BoardPosition from;
    BoardPosition to;
    std::string toString(ChessBoard board);
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_