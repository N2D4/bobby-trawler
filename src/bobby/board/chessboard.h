#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "square/boardsquare.h"
#include "boardposition.h"
#include "boardmoves.h"

struct BoardMove;
struct DetailedMove;

class ChessBoard {
    private:
        bool isCheck(BoardSquare::Color color);


    public:
        BoardSquare squares[8][8];
        BoardSquare::Color curColor;

        ChessBoard();
        BoardSquare& operator[](BoardPosition position);

        void move(DetailedMove move);
        void revert(DetailedMove move);

        bool isCheck();
        bool isLegal(DetailedMove move);

        std::string toHumanReadable(bool ansi=false);
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_