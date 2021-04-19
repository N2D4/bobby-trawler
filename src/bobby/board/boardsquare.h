#ifndef BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_
#define BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_

#include <iostream>
#include "../various/intstruct.h"

struct BoardSquares;

/**
 * A structure representing either a single piece, or an empty square.
 * 
 * A board square can be either one of the six pieces of one of the two colors, or empty. Internally, it is stored as
 * an integer in the binary format 0bBWPKBRQK, whereas one of WB is set for white/black, and one of PKBRQK is set for
 * the piece type. The empty square is stored as 0. This allows some efficient binary trickery.
 */
struct BoardSquare : IntStruct {
    friend BoardSquares;

    private:
        constexpr BoardSquare(int data) : IntStruct(data) { }

        static constexpr const int COLOR_MASK = 0b11 << 6;
        static constexpr const int TYPE_MASK = 0b111111;


    public:
        struct Color : IntStruct {
            friend BoardSquare;
            friend BoardSquares;

            private:
                constexpr Color(int data) : IntStruct(data) { }

            public:
                template<class T>
                struct Container {
                    public:
                        T white;
                        T black;
                        constexpr Container<T>(T white, T black) : white(white), black(black) {}
                        inline T operator[](const BoardSquare::Color color) const { return color.id() == 0 ? white : black; }
                        inline T& operator[](const BoardSquare::Color color) { return color.id() == 0 ? white : black; }
                };
                constexpr int id() const { return this->data == 0 ? -1 : (__builtin_clz(data) - __builtin_clz(0b10000000)); }
                constexpr Color operator!() const { return Color(this->data ^ BoardSquare::COLOR_MASK); }
        };

        struct Type : IntStruct {
            friend BoardSquare;
            friend BoardSquares;

            private:
                constexpr Type(int data) : IntStruct(data) { }

            public:
                constexpr int id() const { return this->data == 0 ? -1 : __builtin_ctz(data); }
                constexpr float getScore() const { return this->data == 0 ? 0 : ((int[6]) {0, 9, 5, 3, 3, 1})[__builtin_ctz(this->data)]; }
                constexpr BoardSquare ofColor(BoardSquare::Color color) const { return BoardSquare(color, *this); }
        };

        constexpr BoardSquare() : BoardSquare(0) { }
        constexpr BoardSquare(Color color, Type type) : BoardSquare(color.data | type.data) { }

        /**
         * A function returning 0 for white pieces, 1 for black pieces, and -1 for empty squares. In most cases,
         * BoardSquare::color() should be preferred.
         */
        constexpr int colorId() const { return isEmpty() ? -1 : (__builtin_clz(data) - __builtin_clz(0b10000000)); }
        /**
         * A function returning an integer determining the piece type; 0 for king, 1 for queen, 2 for
         * rook, 3 for bishop, 4 for knight, 5 for pawn, and -1 for empty square. In most cases, BoardSquare::type()
         * should be preferred.
         */
        constexpr int typeId() const { return isEmpty() ? -1 : __builtin_ctz(data); }
        constexpr Color color() const { return data & COLOR_MASK; }
        constexpr Type type() const { return data & TYPE_MASK; }

        constexpr bool isEmpty() const { return this->data == 0; }
        inline std::string asUnicode() const { if (isEmpty()) return " "; std::string str; str.push_back(0xE2); str.push_back(0x99); str.push_back(0x94 + 6 * colorId() + typeId()); return str; }
};


struct BoardSquares {
    struct Types {
        static constexpr const BoardSquare::Type EMPTY  = 0b000000;
        static constexpr const BoardSquare::Type PAWN   = 0b100000;
        static constexpr const BoardSquare::Type KNIGHT = 0b010000;
        static constexpr const BoardSquare::Type BISHOP = 0b001000;
        static constexpr const BoardSquare::Type ROOK   = 0b000100;
        static constexpr const BoardSquare::Type QUEEN  = 0b000010;
        static constexpr const BoardSquare::Type KING   = 0b000001;
    };

    struct Colors {
        static constexpr const BoardSquare::Color EMPTY = 0b00000000;
        static constexpr const BoardSquare::Color WHITE = 0b10000000;
        static constexpr const BoardSquare::Color BLACK = 0b01000000;
    };

    static constexpr const BoardSquare EMPTY        = 0;
    static constexpr const BoardSquare WHITE_PAWN   = Colors::WHITE.data | Types::PAWN.data;
    static constexpr const BoardSquare BLACK_PAWN   = Colors::BLACK.data | Types::PAWN.data;
    static constexpr const BoardSquare WHITE_KNIGHT = Colors::WHITE.data | Types::KNIGHT.data;
    static constexpr const BoardSquare BLACK_KNIGHT = Colors::BLACK.data | Types::KNIGHT.data;
    static constexpr const BoardSquare WHITE_BISHOP = Colors::WHITE.data | Types::BISHOP.data;
    static constexpr const BoardSquare BLACK_BISHOP = Colors::BLACK.data | Types::BISHOP.data;
    static constexpr const BoardSquare WHITE_ROOK   = Colors::WHITE.data | Types::ROOK.data;
    static constexpr const BoardSquare BLACK_ROOK   = Colors::BLACK.data | Types::ROOK.data;
    static constexpr const BoardSquare WHITE_QUEEN  = Colors::WHITE.data | Types::QUEEN.data;
    static constexpr const BoardSquare BLACK_QUEEN  = Colors::BLACK.data | Types::QUEEN.data;
    static constexpr const BoardSquare WHITE_KING   = Colors::WHITE.data | Types::KING.data;
    static constexpr const BoardSquare BLACK_KING   = Colors::BLACK.data | Types::KING.data;
};


#endif  // BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_


