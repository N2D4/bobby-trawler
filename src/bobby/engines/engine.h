#ifndef BOBBY_TRAWLER_BOBBY_ENGINE_H_
#define BOBBY_TRAWLER_BOBBY_ENGINE_H_

#include <vector>
#include <stdlib.h>
#include "../board/chessboard.h"

/**
 * Abstract class containing virtual methods for a chess engine.
 */
class ChessEngine {
    public:
        struct CalculatedMove {
            BoardMove move;
            float score;
            int depth;
            int selectiveDepth;
            long movesAnalyzed;

            constexpr CalculatedMove(const BoardMove move, const float score, const int movesAnalyzed, const int depth) : CalculatedMove(move, score, movesAnalyzed, depth, depth) { }
            constexpr CalculatedMove(const BoardMove move, const float score, const int movesAnalyzed, const int depth, const int selectiveDepth) : move(move), score(score), movesAnalyzed(movesAnalyzed), depth(depth), selectiveDepth(selectiveDepth) { }
        };

        virtual CalculatedMove findBestMove() = 0;

        virtual int getMemoizationCount() const = 0;
        virtual void resetMemoizations() = 0;
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_