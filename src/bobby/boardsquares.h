#ifndef BOBBY_TRAWLER_BOARDSQUARES_H_
#define BOBBY_TRAWLER_BOARDSQUARES_H_


typedef int BoardSquare;
    // Board square format: 0bBW_PNBRQK. This way we can do some bit tricking for single-instruction operations
    #define EMPTY_SQUARE    0b00_
    #define COLOR_MASK      0b11_      << 6
    #define WHITE_MASK      0b10_      << 6
    #define BLACK_MASK      0b01_      << 6
    #define PIECE_MASK        0b_111111
    #define PAWN_MASK         0b_100000
    #define KNIGHT_MASK       0b_010000
    #define BISHOP_MASK       0b_001000
    #define ROOK_MASK         0b_000100
    #define QUEEN_MASK        0b_000010
    #define KING_MASK         0b_000001

    #define COLOR_MASK_OF(sq)       sq & COLOR_MASK
    #define PIECE_MASK_OF(sq)       sq & PIECE_MASK
    #define IS_WHITE(sq)            sq & WHITE_MASK
    #define IS_BLACK(sq)            sq & BLACK_MASK 
    #define IS_PAWN(sq)             sq & PAWN_MASK
    #define IS_KNIGHT(sq)           sq & KNIGHT_MASK
    #define IS_BISHOP(sq)           sq & BISHOP_MASK
    #define IS_ROOK(sq)             sq & ROOK_MASK
    #define IS_QUEEN(sq)            sq & QUEEN_MASK
    #define IS_KING(sq)             sq & KING_MASK

    #define WHITE_PAWN      WHITE_MASK | PAWN_MASK
    #define BLACK_PAWN      BLACK_MASK | PAWN_MASK
    #define WHITE_KNIGHT    WHITE_MASK | KNIGHT_MASK
    #define BLACK_KNIGHT    BLACK_MASK | KNIGHT_MASK
    #define WHITE_BISHOP    WHITE_MASK | BISHOP_MASK
    #define BLACK_BISHOP    BLACK_MASK | BISHOP_MASK
    #define WHITE_ROOK      WHITE_MASK | ROOK_MASK
    #define BLACK_ROOK      BLACK_MASK | ROOK_MASK
    #define WHITE_QUEEN     WHITE_MASK | QUEEN_MASK
    #define BLACK_QUEEN     BLACK_MASK | QUEEN_MASK
    #define WHITE_KING      WHITE_MASK | KING_MASK
    #define BLACK_KING      BLACK_MASK | KING_MASK





#endif  // BOBBY_TRAWLER_BOARDSQUARES_H_


