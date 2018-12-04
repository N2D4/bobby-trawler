#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "square/boardsquare.h"
#include "boardposition.h"
#include "boardmoves.h"

struct BoardMove;
struct BoardSquare;
struct DetailedMove;

/**
 * A class representing a chess board.
 * 
 * A chess board contains of 64 squares, each square representing either an empty square or a piece.
 * 
 * Note that it is not only perfectly normal but also encouraged to use the move(...) and revert() methods in
 * conjunction and recursively eg. when analyzing a potential future board state. However, by convention, at the end
 * of every function call the board state should be reverted back to the beginning state, unless that function is
 * explicitly supposed to modify the board state (such as the move function itself).
 */
class ChessBoard {
    private:
        float materialScore;
        bool isLineEmpty(BoardPosition pos1, BoardPosition pos2);


    public:
        BoardSquare squares[8][8];
        BoardSquare::Color curColor;
        std::vector<DetailedMove> moves;
        BoardSquare::Color::Container<bool> allowCastlingKingside, allowCastlingQueenside;
        BoardSquare::Color::Container<BoardPosition> kingPos;

        ChessBoard();
        BoardSquare& operator[](BoardPosition position);

        DetailedMove createDetailedMove(BoardMove move);

        void move(BoardMove move);
        void move(DetailedMove move);
        void revert();

        bool isCheck();
        bool isCheck(BoardSquare::Color color);
        bool isSquareAttacked(BoardSquare::Color color, BoardPosition position);
        bool isLegal(BoardMove move);

        float getMaterialScore();
        float getMaterialScore(BoardSquare::Color color);

        std::string toHumanReadable(bool ansi=false);
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_