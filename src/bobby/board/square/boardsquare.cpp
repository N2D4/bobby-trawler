#include "boardsquare.h"


const int BoardSquare::COLOR_MASK                    = 0b11      << 6;
const BoardSquare::Color BoardSquare::Color::WHITE   = 0b10      << 6;
const BoardSquare::Color BoardSquare::Color::BLACK   = 0b01      << 6;
const int BoardSquare::PIECE_MASK                    =   0b111111;
const BoardSquare::Type BoardSquare::Type::PAWN      =   0b100000;
const BoardSquare::Type BoardSquare::Type::KNIGHT    =   0b010000;
const BoardSquare::Type BoardSquare::Type::BISHOP    =   0b001000;
const BoardSquare::Type BoardSquare::Type::ROOK      =   0b000100;
const BoardSquare::Type BoardSquare::Type::QUEEN     =   0b000010;
const BoardSquare::Type BoardSquare::Type::KING      =   0b000001;


const BoardSquare BoardSquare::EMPTY = 0;
const BoardSquare BoardSquare::WHITE_PAWN   = Color::WHITE.data | Type::PAWN.data;
const BoardSquare BoardSquare::BLACK_PAWN   = Color::BLACK.data | Type::PAWN.data;
const BoardSquare BoardSquare::WHITE_KNIGHT = Color::WHITE.data | Type::KNIGHT.data;
const BoardSquare BoardSquare::BLACK_KNIGHT = Color::BLACK.data | Type::KNIGHT.data;
const BoardSquare BoardSquare::WHITE_BISHOP = Color::WHITE.data | Type::BISHOP.data;
const BoardSquare BoardSquare::BLACK_BISHOP = Color::BLACK.data | Type::BISHOP.data;
const BoardSquare BoardSquare::WHITE_ROOK   = Color::WHITE.data | Type::ROOK.data;
const BoardSquare BoardSquare::BLACK_ROOK   = Color::BLACK.data | Type::ROOK.data;
const BoardSquare BoardSquare::WHITE_QUEEN  = Color::WHITE.data | Type::QUEEN.data;
const BoardSquare BoardSquare::BLACK_QUEEN  = Color::BLACK.data | Type::QUEEN.data;
const BoardSquare BoardSquare::WHITE_KING   = Color::WHITE.data | Type::KING.data;
const BoardSquare BoardSquare::BLACK_KING   = Color::BLACK.data | Type::KING.data;


BoardSquare::BoardSquare() : IntStruct(BoardSquare::EMPTY) { }
BoardSquare::BoardSquare(int data) : IntStruct(data) { }
BoardSquare::Type::Type(int data) : IntStruct(data) { }
BoardSquare::Color::Color(int data) : IntStruct(data) { }

bool BoardSquare::isEmpty() {
    return *this == BoardSquare::EMPTY;
}

std::string BoardSquare::asUnicode() {
    if (*this == BoardSquare::EMPTY) {
        return " ";
    } else {
        std::string res;
        res.push_back(0xE2);
        res.push_back(0x99);
        int sq = this->data;
        res.push_back((0x94 + 6 * (__builtin_clz(sq) - __builtin_clz(BoardSquare::Color::WHITE.data)) + __builtin_ctz(sq)));
        return res;
    }
}
