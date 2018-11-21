#ifndef BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_
#define BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_

#include <iostream>
#include <vector>
#include "square/boardsquare.h"

struct BoardPosition {
    int column;
    int row;

    BoardPosition(int column, int row);

    bool operator==(const BoardPosition& w);
    bool operator!=(const BoardPosition& w);

    bool isValid();
    
    // Implicit casting from/to C++ strings
    BoardPosition(const std::string& str);
    BoardPosition& operator= (const std::string& str);
    operator std::string();

    // Implicit casting from C strings (for literals)
    BoardPosition(const char* str);
    BoardPosition& operator= (const char* str);
};

#endif  // BOBBY_TRAWLER_BOBBY_BOARDPOSITION_H_