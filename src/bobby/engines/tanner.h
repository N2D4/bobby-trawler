#ifndef BOBBY_TRAWLER_BOBBY_ENGINES_TANNER_H_
#define BOBBY_TRAWLER_BOBBY_ENGINES_TANNER_H_

#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <random>
#include "../board/chessboard.h"
#include "engine.h"

/**
 * The Tanner brute-force engine.
 */
class TannerEngine : ChessEngine {
    private:
        std::unordered_map<ChessBoard::CacheName, std::pair<float, int>> memoizedPositions;

    public:
        ChessBoard& board;
        static int cacheHits[33];
        static int cacheCalls[33];

        TannerEngine(ChessBoard& board);

        std::tuple<float, int, BoardMove> findBestMove();
        std::tuple<float, int, BoardMove> findBestMove(int depth);

        int getMemoizationCount() const;
        void resetMemoizations();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINES_TANNER_H_