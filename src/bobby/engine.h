#ifndef BOBBY_TRAWLER_BOBBY_ENGINE_H_
#define BOBBY_TRAWLER_BOBBY_ENGINE_H_

#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <random>
#include "board/chessboard.h"

/**
 * The class containing the code for the Bobby Trawler chess engine itself.
 */
class ChessEngine {
    private:
        std::unordered_map<ChessBoard::CacheName, std::pair<float, int>> memoizedPositions;

    public:
        ChessBoard& board;
        static int cacheHits[33];
        static int cacheCalls[33];

        ChessEngine(ChessBoard& board);

        std::tuple<float, int, BoardMove> findBestMove();
        std::tuple<float, int, BoardMove> findBestMove(int depth);

        int getMemoizationCount() const;
        void resetMemoizations();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_