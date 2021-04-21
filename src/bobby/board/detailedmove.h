#ifndef BOBBY_TRAWLER_BOBBY_BOARD_DETAILEDMOVE_H_
#define BOBBY_TRAWLER_BOBBY_BOARD_DETAILEDMOVE_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "boardmoves.h"
#include "boardposition.h"

class ChessBoard;

/**
 * A fairly complex struct representing a chess move from some position from to a position to, with some additional
 * information related to the board state required to revert the board state back, such as the piece captured.
 * 
 * It is mostly used inside ChessBoard.
 */
struct DetailedMove : BoardMove {
    friend ChessBoard;

    public:
        enum MoveType : unsigned char {
            NORMAL = 0, PAWN_DOUBLE = 1, ENPASSANT = 2, PROMOTION = 3, CASTLING_KINGSIDE = 4, CASTLING_QUEENSIDE = 5,
        };

        BoardSquare captured;
        MoveType type;
        int prevFlags;

    private:
        constexpr DetailedMove(BoardPosition from, BoardPosition to, BoardSquare::Type promoteTo, BoardSquare captured, MoveType type, int prevFlags) : BoardMove(from, to, promoteTo), captured(captured), type(type), prevFlags(prevFlags) { }
    
    public:
        /**
         * see BoardMove::isPossibleFor(BoardSquare, bool)
         */
        inline bool isPossibleFor(BoardSquare square) const { return BoardMove::isPossibleFor(square, this->captured != BoardSquares::EMPTY); }
        constexpr bool isCastling() const { return this->type >= DetailedMove::MoveType::CASTLING_KINGSIDE; }
};


#endif  // BOBBY_TRAWLER_BOBBY_BOARD_DETAILEDMOVE_H_
