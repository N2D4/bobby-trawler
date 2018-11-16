#include <iostream>
#include <vector>
#include "boardsquares.cpp"
using namespace std;



struct ChessBoard {
    BoardSquare squares[8][8];

    ChessBoard() {
        // TODO Set up the board
    }
};




struct BoardPosition {
    int column;
    int row;

    string toString() {
        string str;
        str.push_back('a' + column);
        return str + to_string(row + 1);
    }

    string getFigureAbbreviation() {
        // Example: R (for rook)
        // TODO
        return string();
    }
};




struct ChessMove {
    BoardPosition from;
    BoardPosition to;

    string toString(ChessBoard board) {
        // Example: "e4e5"
        // TODO
        return string();
    }
};