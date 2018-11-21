#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "square/boardsquare.h"
#include "boardposition.h"
#include "boardmoves.h"

class ChessBoard {
    public:
        BoardSquare squares[8][8];
        BoardSquare::Color curColor;

        ChessBoard();
        BoardSquare& operator[](BoardPosition position);

        void move(BoardMove move);

        bool isCheck();
        bool isLegal(BoardMove move);

        std::string toHumanReadable(bool ansi=false);
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_