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
        virtual std::tuple<float, int, BoardMove> findBestMove() = 0;
        virtual std::tuple<float, int, BoardMove> findBestMove(int depth) = 0;

        virtual int getMemoizationCount() const = 0;
        virtual void resetMemoizations() = 0;
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_