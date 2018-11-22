#ifndef BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_
#define BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "boardposition.h"
#include "chessboard.h"

struct DetailedMove;
class ChessBoard;

struct BoardMove {
    BoardPosition from;
    BoardPosition to;

    BoardMove(BoardPosition from, BoardPosition to);

    bool operator==(const BoardMove& w);
    bool operator!=(const BoardMove& w);
    
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


struct DetailedMove : BoardMove {
    friend BoardMove;

    private:
        DetailedMove(BoardPosition from, BoardPosition to, BoardSquare captured);
    
    public:
        BoardSquare captured;
        bool isPossibleFor(BoardSquare square);
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_