#ifndef BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_
#define BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_

#include <iostream>
#include "various/intstruct.h"

/**
 * A structure representing either a single figure, or an empty square.
 * 
 * A board square can be either one of the six figures of one of the two colors, or empty. Internally, it is stored as
 * an integer in the binary format 0bBWPKBRQK, whereas one of WB is set for white/black, and one of PKBRQK is set for
 * the figure type. The empty square is stored as 0. This allows some efficient binary trickery.
 */
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
        /**
         * A function returning 0 for white figures, 1 for black figures, and -1 for empty squares. In most cases,
         * BoardSquare::color() should be preferred.
         */
        int colorId();
        /**
         * A function returning an integer determining the figure type; 0 for king, 1 for queen, 2 for
         * rook, 3 for bishop, 4 for knight, 5 for pawn, and -1 for empty square. In most cases, BoardSquare::type()
         * should be preferred.
         */
        int typeId();

        bool isEmpty();
        std::string asUnicode();
};

#endif  // BOBBY_TRAWLER_SQUARE_BOARDSQUARES_H_


