#include "chessboard.h"



ChessBoard::ChessBoard() {
    for (int i = 0; i < 8; i++) {
        this->squares[i][1] = WHITE_PAWN;
        this->squares[i][6] = BLACK_PAWN;
    }
    (*this)["a1"] = (*this)["h1"] = WHITE_ROOK;
    (*this)["a8"] = (*this)["h8"] = BLACK_ROOK;
    (*this)["b1"] = (*this)["g1"] = WHITE_KNIGHT;
    (*this)["b8"] = (*this)["g8"] = BLACK_KNIGHT;
    (*this)["c1"] = (*this)["f1"] = WHITE_BISHOP;
    (*this)["c8"] = (*this)["f8"] = BLACK_BISHOP;
    (*this)["d1"] = WHITE_QUEEN;
    (*this)["d8"] = BLACK_QUEEN;
    (*this)["e1"] = WHITE_KING;
    (*this)["e8"] = BLACK_KING;
}

inline BoardSquare& ChessBoard::operator[](BoardPosition position) {
    return this->squares[position.column][position.row];
}

bool ChessBoard::isLegal(BoardMove move) {
    BoardSquare square = (*this)[move.from];
    if (square == EMPTY_SQUARE) return false;

    return true; // TODO
}

std::string ChessBoard::toHumanReadable(bool ansi) {
    std::string result;
    for (int i = 7; i >= 0; i--) {
        result.push_back('1' + i);
        result.push_back(' ');
        for (int j = 0; j < 8; j++) {
            BoardSquare sq = this->squares[j][i];
            if (ansi) result += (i + j) % 2 == 0 ? "\033[48;5;249m" : "\033[48;5;253m";
            result.push_back(' ');
            result += AS_UNICODE(sq);
            result.push_back(' ');
            if (ansi) result += "\033[0m";
        }
        result.push_back('\n');
    }
    result += "  ";
    for (int j = 0; j < 8; j++) {
        result.push_back(' ');
        result.push_back('a' + j);
        result.push_back(' ');
    }
    return result;
}











BoardPosition::BoardPosition(int column, int row) {
    this->column = column;
    this->row = row;
}


BoardPosition::BoardPosition(const std::string& str) {
    *this = str;
}

inline BoardPosition& BoardPosition::operator= (const std::string& str) {
    this->column = str[0] - 'a';
    this->row = str[1] - '1';
    return *this;
}

inline BoardPosition::operator std::string() {
    char s[3] = {(char) ('a' + column),(char) ('1' + row), '\0'};
    return std::string(s);
}


BoardPosition::BoardPosition(const char* str) {
    *this = str;
}

inline BoardPosition& BoardPosition::operator= (const char* str) {
    this->column = str[0] - 'a';
    this->row = str[1] - '1';
    return *this;
}








BoardMove::BoardMove(BoardPosition from, BoardPosition to) : from(from), to(to) { }