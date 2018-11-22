#ifndef BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_
#define BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "boardposition.h"
#include "chessboard.h"

struct DetailedMove;
class ChessBoard;

/**
 * A simple struct representing a chess move from some position from to a position to.
 * 
 * This simple move does not store any information about the move itself or the board state before or after the move;
 * it is simply a struct that could represent user input. It is not connected to any board or game state in any way.
 * 
 * In order to use a BoardMove object to a chess board, it must be converted to a DetailedMove object using the
 * detailed(...) method.
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

    DetailedMove detailed(ChessBoard& board);
    

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
 * This structure can only be created by calling the .detailed(...) method of BoardMove. If used on any board other than
 * the board used to create this DetailedMove (or the board's state has changed), behaviour is undefined.
 */
struct DetailedMove : BoardMove {
    friend BoardMove;

    private:
        DetailedMove(BoardPosition from, BoardPosition to, BoardSquare captured);
    
    public:
        BoardSquare captured;
        /**
         * see BoardMove::isPossibleFor(BoardSquare, bool)
         */
        bool isPossibleFor(BoardSquare square);
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_