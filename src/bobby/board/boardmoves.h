#ifndef BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_
#define BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_

#include <iostream>
#include <vector>
#include <stdexcept>
#include "boardposition.h"

struct BoardMove {
    BoardPosition from;
    BoardPosition to;

    BoardMove(BoardPosition from, BoardPosition to);

    bool operator==(const BoardMove& w);
    bool operator!=(const BoardMove& w);
    
    bool isPossibleFor(BoardSquare square, bool isCapture);
    

    // Implicit casting from/to C++ strings
    BoardMove(const std::string& str);
    BoardMove& operator= (const std::string& str);
    operator std::string();

    // Implicit casting from C strings (for literals)
    BoardMove(const char* str);
    BoardMove& operator= (const char* str);
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDMOVES_H_