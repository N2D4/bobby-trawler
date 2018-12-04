#ifndef BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_
#define BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "square/boardsquare.h"
#include "boardposition.h"
#include "chessboard.h"

struct DetailedMove;
struct BoardSquare;
class ChessBoard;

/**
 * A simple struct representing a chess move from some position from to a position to.
 * 
 * This simple move does not store any information about the move itself or the board state before or after the move;
 * it is simply a struct that could represent user input. It is not connected to any board or game state in any way.
 */
struct BoardMove {
    BoardPosition from;
    BoardPosition to;

    BoardMove(BoardPosition from, BoardPosition to);

    bool operator==(const BoardMove& w);
    bool operator!=(const BoardMove& w);
    
    /**
     * Returns a boolean value indicating whether the given BoardSquare piece can move in that shape. In case that
     * the piece is a pawn, the second argument isCapture is used to determine whether it can move diagonally or not.
     * If the square is empty, false will be returned.
     */
    bool isPossibleFor(BoardSquare square, bool isCapture);


    // Implicit casting from/to C++ strings
    BoardMove(const std::string& str);
    BoardMove& operator= (const std::string& str);
    operator std::string();

    // Implicit casting from C strings (for literals)
    BoardMove(const char* str);
    BoardMove& operator= (const char* str);
};


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

    private:
        DetailedMove(BoardPosition from, BoardPosition to, BoardSquare captured, MoveType type);

    
    public:
        BoardSquare captured;
        MoveType type;

        /**
         * see BoardMove::isPossibleFor(BoardSquare, bool)
         */
        bool isPossibleFor(BoardSquare square);
        bool isCastling();
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_