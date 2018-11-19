#include "boardposition.h"


BoardPosition::BoardPosition(int column, int row) {
    this->column = column;
    this->row = row;
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