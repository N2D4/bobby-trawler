#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include "boardsquare.h"
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
        int pieceCount;
        bool isLineEmpty(BoardPosition pos1, BoardPosition pos2);
        static int cacheNameHits[33];
        static int cacheNameCalls[33];
        unsigned long long _priv_getUniqueCacheName() const;

    public:
        BoardSquare squares[8][8];
        BoardSquare::Color curColor;
        std::vector<DetailedMove> moves;
        BoardSquare::Color::Container<bool> allowCastlingKingside, allowCastlingQueenside;
        BoardSquare::Color::Container<BoardPosition> kingPos;

        ChessBoard();
        constexpr BoardSquare operator[](BoardPosition position) const { return this->squares[position.column][position.row]; }
        inline BoardSquare& operator[](BoardPosition position) { return this->squares[position.column][position.row]; }

        DetailedMove createDetailedMove(BoardMove move) const;

        void move(BoardMove move);
        void move(DetailedMove move);
        void revert();

        bool isCheck();
        bool isCheck(BoardSquare::Color color);
        bool isSquareAttacked(BoardSquare::Color color, BoardPosition position) const;
        bool isLegal(BoardMove move);

        constexpr float getMaterialScore() const { return this->materialScore; }
        constexpr float getMaterialScore(BoardSquare::Color color) const { return color == BoardSquares::Colors::WHITE ? getMaterialScore() : -getMaterialScore(); }
        constexpr int getPieceCount() const { return this->pieceCount; }

        /**
         * Returns a value unique to this board position. If it is 0, no unique value could've been created. Usually
         * returns a non-zero value when there are only few pieces on the board. There are multiple reasons why it may
         * fail to create a unique value; for example if there are many pieces on the board or one of the kings may
         * still castle. As a rule of thumb, it succeeds to generate a unique value for most board positions with 7 or
         * less pieces, but might still fail even in those situations.
         */
        unsigned long long getUniqueCacheName() const;

        std::string toHumanReadable(bool ansi=false) const;
        std::string getInfo(bool ansi=false) const;
};

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_