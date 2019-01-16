#ifndef BOBBY_TRAWLER_BOBBY_ENGINES_DAEDRIAN_H_
#define BOBBY_TRAWLER_BOBBY_ENGINES_DAEDRIAN_H_

#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <random>
#include "../board/chessboard.h"
#include "engine.h"

/**
 * The Daedrian selective minimax engine.
 */
class DaedrianEngine : ChessEngine {
    private:
        std::unordered_map<ChessBoard::CacheName, std::pair<float, int>> memoizedPositions;

        ChessEngine::CalculatedMove findBestMove(float selwidth, float seldec, int depth, int nextDepth);
        std::vector<std::pair<float, BoardMove>> findCandidates(int depth, int count, long& totalBottomLayerMoves);

    public:
        ChessBoard& board;
        static int cacheHits[33];
        static int cacheCalls[33];

        DaedrianEngine(ChessBoard& board);

        ChessEngine::CalculatedMove findBestMove();

        int getMemoizationCount() const;
        void resetMemoizations();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINES_DAEDRIAN_H_