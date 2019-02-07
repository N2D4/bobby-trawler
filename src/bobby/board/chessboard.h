#ifndef BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_
#define BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_

#include <iostream>
#include <vector>
#include <cstdint>
#include <iterator>
#include <functional>
#include "boardsquare.h"
#include "boardposition.h"
#include "boardmoves.h"
#include "detailedmove.h"

struct BoardSquare;
struct DetailedMove;

/**
 * A class representing a chess board.
 * 
 * A chess board contains of 64 squares, each square representing either an empty square or a piece.
 * 
 * Note that it is not only perfectly normal but also encouraged to use the move(...) and revert() methods in
 * conjunction and recursively eg. when analyzing a potential future board state. However, by convention, at the end
 * of every function call the board state should be reverted back to the beginning state, unless that function is
 * explicitly supposed to modify the board state (such as the move function itself).
 */
class ChessBoard {
    public: 
        struct CacheName {
            uint64_t a;
            uint64_t b;
            uint64_t c;

            constexpr CacheName() : CacheName(0, 0, 0) { }
            constexpr CacheName(uint64_t a, uint64_t b, uint64_t c) : a(a), b(b), c(c) { }
            constexpr bool operator==(const CacheName& w) const { return a == w.a && b == w.b && c == w.c; }
            constexpr bool operator!=(const CacheName& w) const { return !(*this == w); }
            constexpr bool isInvalid() const { return a == 0 && b == 0 && c == 0; }
            inline operator std::string() const { return std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c); }
        };
    
    private:
        float materialScore;
        int pieceCount;
        bool isLineEmpty(BoardPosition pos1, BoardPosition pos2);
        static int cacheNameHits[33];
        static int cacheNameCalls[33];
        CacheName _priv_getUniqueCacheName() const;
        unsigned long long _priv_getShortUniqueCacheName() const;

    public:
        BoardSquare squares[8][8];
        BoardSquare::Color curColor;
        std::vector<DetailedMove> moves;
        BoardSquare::Color::Container<bool> allowCastlingKingside, allowCastlingQueenside;
        BoardSquare::Color::Container<BoardPosition> kingPos;

        ChessBoard();
        constexpr BoardSquare operator[](BoardPosition position) const { return this->squares[position.column][position.row]; }
        inline BoardSquare& operator[](BoardPosition position) { return this->squares[position.column][position.row]; }

        DetailedMove createDetailedMove(const BoardMove move) const;

        inline void move(const BoardMove move) { moveDetailed(createDetailedMove(move)); }
        void moveDetailed(const DetailedMove& move);
        void revert();

        /*class MoveIterator : public std::iterator<std::input_iterator_tag, BoardMove> {
            private:
                constexpr MoveIterator() { }
            public:
                static MoveIterator end;
                virtual MoveIterator& operator++();
                virtual MoveIterator operator++(int);
                constexpr bool operator==(const MoveIterator& rhs) const { return (*this == end && rhs == end); }
                constexpr bool operator!=(const MoveIterator& rhs) const { return !(*this == rhs); }
                virtual BoardMove& operator*();

        };
        class LineMoveIterator : MoveIterator {
            private:
                ChessBoard& board;
                BoardMove move;
                int dx, dy;
                int rem;
            public:
                constexpr LineMoveIterator(ChessBoard& board, const BoardPosition pos, const int rem) : board(board), move(pos, pos), rem(rem), dx(0), dy(0) { nextRem(); operator++(); }
                constexpr LineMoveIterator(const LineMoveIterator& it) : board(it.board), move(it.move), rem(it.rem), dx(it.dx), dy(it.dy) { }
                inline void nextRem() { rem &= rem - 1; int r = __builtin_ctz(rem); dx = r % 3 - 1; dy = r / 3 - 1; }
                MoveIterator& operator++() override;
                inline MoveIterator operator++(int) override { LineMoveIterator tmp(*this); operator++(); return tmp; }
                BoardMove& operator*() override { return move; }
        };
        template <int N>
        class ArrayMoveIterator : MoveIterator {
            private:
                ChessBoard& board;
                BoardPosition[N] arr;
        };*/
        //MoveIterator getMoves(const BoardPosition pos) const;
        void forEachMove(const BoardPosition pos, std::function<void (BoardMove)> func);

        inline bool isCheck() const { return isCheck(this->curColor); }
        inline bool isCheck(const BoardSquare::Color color) const { return isSquareAttacked(!color, this->kingPos[color]); }
        bool isSquareAttacked(const BoardSquare::Color color, BoardPosition position) const;
        bool isLegal(const BoardMove move);

        constexpr float getMaterialScore() const { return this->materialScore; }
        constexpr float getMaterialScore(BoardSquare::Color color) const { return color == BoardSquares::Colors::WHITE ? getMaterialScore() : -getMaterialScore(); }
        constexpr int getPieceCount() const { return this->pieceCount; }

        /**
         * Returns a value unique to this board position. If it is 0, no unique value could've been created. Usually
         * returns a non-zero value when there are only few pieces on the board. There are multiple reasons why it may
         * fail to create a unique value; for example if there are many pieces on the board or one of the kings may
         * still castle. As a rule of thumb, it succeeds to generate a unique value for most board positions with 7 or
         * less pieces, but might still fail even in those situations.
         */
        CacheName getUniqueCacheName() const;

        std::string toHumanReadable(bool ansi=false) const;
        std::string toHumanReadable(std::vector<std::tuple<BoardPosition, int, int>>, bool ansi=false) const;
        std::string getInfo(bool ansi=false) const;
};

namespace std {
    template <>
    struct hash<ChessBoard::CacheName>
    {
        std::size_t operator()(const ChessBoard::CacheName& k) const {
            return 31 * (31 * (713 + k.a) + k.b) + k.c;
        }
    };
}

#endif  // BOBBY_TRAWLER_BOBBY_CHESSBOARD_H_