#ifndef BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_
#define BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "boardsquare.h"
#include "boardposition.h"

struct BoardSquare;

constexpr static BoardSquare::Type bmctotype(const char c) {
    switch (c) {
        case 'Q': return BoardSquares::Types::QUEEN;
        case 'R': return BoardSquares::Types::ROOK;
        case 'B': return BoardSquares::Types::BISHOP;
        case 'N': return BoardSquares::Types::KNIGHT;
    }
    return BoardSquares::Types::QUEEN; 
}

inline static BoardSquare::Type bmstrtotype(const std::string& str) {
    if (str.size() >= 5) {
        return bmctotype(str[4]);
    }
    return BoardSquares::Types::QUEEN;
}

/**
 * A simple struct representing a chess move from some position from to a position to.
 * 
 * This simple move does not store any information about the move itself or the board state before or after the move;
 * it is simply a struct that could represent user input. It is not connected to any board or game state in any way.
 */
struct BoardMove {
    BoardPosition from;
    BoardPosition to;
    BoardSquare::Type promoteTo;

    constexpr BoardMove(BoardPosition from, BoardPosition to, BoardSquare::Type promoteTo) : from(from), to(to), promoteTo(promoteTo) { }
    constexpr BoardMove(BoardPosition from, BoardPosition to) : BoardMove(from, to, BoardSquares::Types::QUEEN) { }

    constexpr bool operator==(const BoardMove& w) const { return from == w.from && to == w.to && promoteTo == w.promoteTo; }
    constexpr bool operator!=(const BoardMove& w) const { return !(*this == w); }
    
    /**
     * Returns a boolean value indicating whether the given BoardSquare piece can move in that shape. In case that
     * the piece is a pawn, the second argument isCapture is used to determine whether it can move diagonally or not.
     * If the square is empty, false will be returned.
     */
    bool isPossibleFor(const BoardSquare square, bool isCapture) const;


    // Implicit casting from/to C++ strings
    inline BoardMove(const std::string& str) : BoardMove(str.substr(0, 2), str.substr(2, 2), bmstrtotype(str)) { }
    inline BoardMove& operator=(const std::string& str) { return *this = BoardMove(str); }
    inline operator std::string() const { return std::string(from) + std::string(to) /* TODO: Add promoted piece *//*+ promoteTo == BoardSquares::Types::QUEEN ? "" : promoteTo*/; }

    // Implicit casting from C strings (for literals)
    constexpr BoardMove(const char* str) : BoardMove(str, str+2, bmctotype(str[4])) { }
    inline BoardMove& operator=(const char* str) { return *this = BoardMove(str); }
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_