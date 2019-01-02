#include "boardposition.h"


BoardPosition::BoardPosition(int column, int row) {
    this->column = column;
    this->row = row;
}



bool BoardPosition::isValid() {
    return ((unsigned int) (this->column | this->row)) < 8;
}

float BoardPosition::getPawnBonusScore(BoardSquare::Color color) {
    return std::pow(std::max(0.0, color == BoardSquare::Color::BLACK ? 3.0 - this->row : this->row - 4.0), 2) / 3;
}



bool BoardPosition::operator==(const BoardPosition& w) {
    return this->column == w.column && this->row == w.row;
}
bool BoardPosition::operator!=(const BoardPosition& w) {
    return !(*this == w);
}






// Casting from/to std::string
BoardPosition::BoardPosition(const std::string& str) { *this = str; }
BoardPosition& BoardPosition::operator= (const std::string& str) {
    this->column = str[0] - 'a';
    this->row = str[1] - '1';
    return *this;
}
BoardPosition::operator std::string() {
    char s[3] = {(char) ('a' + column),(char) ('1' + row), '\0'};
    return std::string(s);
}

// Casting from char*
BoardPosition::BoardPosition(const char* str) { *this = str; }
BoardPosition& BoardPosition::operator= (const char* str) {
    this->column = str[0] - 'a';
    this->row = str[1] - '1';
    return *this;
}
