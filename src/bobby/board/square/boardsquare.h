#ifndef BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_
#define BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_

#include <iostream>
#include "various/intstruct.h"


struct BoardSquare : IntStruct {
    private:
        BoardSquare(int data);

        static const int COLOR_MASK;
        static const int TYPE_MASK;


    public:
        struct Color : IntStruct {
            friend BoardSquare;

            private:
                Color(int data);

            public:
                static const Color WHITE, BLACK, EMPTY;
                Color operator!();
        };

        struct Type : IntStruct {
            friend BoardSquare;

            private:
                Type(int data);

            public:
                static const Type PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY;
        };

        static const BoardSquare EMPTY;
        static const BoardSquare WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING;
        static const BoardSquare BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING;

        BoardSquare();
        BoardSquare(Color color, Type type);

        Color color();
        Type type();
        int colorId();
        int typeId();

        bool isEmpty();
        std::string asUnicode();
};

#endif  // BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_


