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
    bool isPossibleFor(BoardSquare square, bool isCapture) const;


    // Implicit casting from/to C++ strings
    inline BoardMove(const std::string& str) : BoardMove(str.substr(0, 2), str.substr(2, 2), bmstrtotype(str)) { }
    inline BoardMove& operator=(const std::string& str) { return *this = BoardMove(str); }
    inline operator std::string() const { return std::string(from) + std::string(to) /* TODO: Add promoted piece *//*+ promoteTo == BoardSquares::Types::QUEEN ? "" : promoteTo*/; }

    // Implicit casting from C strings (for literals)
    constexpr BoardMove(const char* str) : BoardMove((char[]) {str[0], str[1], 0}, (char[]) {str[2], str[3], 0}, bmctotype(str[4])) { }
    inline BoardMove& operator=(const char* str) { return *this = BoardMove(str); }
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

        BoardSquare captured;
        MoveType type;
        int prevFlags;

    private:
        constexpr DetailedMove(BoardPosition from, BoardPosition to, BoardSquare::Type promoteTo, BoardSquare captured, MoveType type, int prevFlags) : BoardMove(from, to, promoteTo), captured(captured), type(type), prevFlags(prevFlags) { }
    
    public:
        /**
         * see BoardMove::isPossibleFor(BoardSquare, bool)
         */
        constexpr bool isPossibleFor(BoardSquare square) const { return BoardMove::isPossibleFor(square, this->captured != BoardSquares::EMPTY); }
        constexpr bool isCastling() const { return this->type >= DetailedMove::MoveType::CASTLING_KINGSIDE; }
};


#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_