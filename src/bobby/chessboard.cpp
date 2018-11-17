#include "chessboard.h"


ChessBoard::ChessBoard() {
    // TODO Set up the board
}

BoardSquare ChessBoard::getSquare(BoardPosition position) {
    return this->squares[position.column][position.row];
}






std::string BoardPosition::toString() {
    std::string str;
    str.push_back('a' + column);
    return str + std::to_string(row + 1);
}

std::string ChessMove::toString(ChessBoard board) {
    // Example: "e4e5"
    // TODO
    return std::string();
}
