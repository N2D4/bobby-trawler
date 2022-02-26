#include "tanner.h"


TannerEngine::TannerEngine(ChessBoard& board) : board(board) { }

int TannerEngine::cacheCalls[33] = {0};
int TannerEngine::cacheHits[33] = {0};

int minDepth = 5;

ChessEngine::CalculatedMove TannerEngine::findBestMove() {
    return findBestMoveWithMaxEffort(2500000);
}

ChessEngine::CalculatedMove TannerEngine::findBestMoveWithMaxEffort(int maxCalculationEffort) {
    int depth = minDepth;
    while (true) {
        ChessEngine::CalculatedMove tup = this->findBestMove(depth);
        // note: if we were to compare tup.movesAnalyzed instead of tup.calculationEffort, undo-ing and then
        // asking the engine again would make the engine increase depth by 1 every time, which is exponentially slow
        if (tup.score > 1000 || depth >= 30 || tup.calculationEffort >= maxCalculationEffort) return tup;
        depth += 1;
    }
}

ChessEngine::CalculatedMove TannerEngine::findBestMove(int depth) {
    static std::default_random_engine rng(421337);
    static std::normal_distribution<float> random(0.0, 0.00002);

    // Check for caching
    ChessBoard::CacheName cacheName = board.getUniqueCacheName();
    if (!cacheName.isInvalid()) {
        int pieceCount = board.getPieceCount();
        if (pieceCount <= 32) {
            cacheCalls[pieceCount]++;
        }
        std::unordered_map<ChessBoard::CacheName, std::tuple<float, int, int, BoardMove>>::iterator cacheEntry = memoizedPositions.find(cacheName);
        int cacheDepth = std::get<1>(cacheEntry->second);
        if (cacheEntry != memoizedPositions.end() && cacheDepth >= depth) {
            // We found a cache entry!
            if (pieceCount <= 32) {
                cacheHits[pieceCount]++;
            }
            BoardMove move = std::get<3>(cacheEntry->second);
            float score = std::get<0>(cacheEntry->second);
            score *= std::pow(0.999, depth - cacheDepth);
            int skippedEffort = std::get<2>(cacheEntry->second);
            return ChessEngine::CalculatedMove(move, score, 0, depth, cacheDepth, skippedEffort);
        }
    }


    float bestScore = NAN;
    BoardMove bestMove = "e2e4";
    int totalBottomLayerMoves = 0;

    // Go through all 64 positions, and through every legal move each
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.squares[i][j].color() != board.curColor) continue;
            board.forEachMove(BoardPosition(i, j), [this, depth, &bestScore, &bestMove, &totalBottomLayerMoves] (BoardMove move) {
                float curScore = 0;
                this->board.move(move);
                {
                    int analyzed = 0;

                    // Calculate board score
                    if (depth > 1) {            // recurse deeper
                        ChessEngine::CalculatedMove cres = this->findBestMove(depth - 1);
                        curScore = cres.score;
                        if (std::abs(curScore) > 1000) curScore *= 0.999;
                        analyzed = cres.movesAnalyzed;
                    } else {                    // base case
                        curScore = this->board.getBoardScore(this->board.curColor);
                        analyzed = 1;

                        // Add some randomness so our computer isn't 100% deterministic
                        curScore += random(rng);
                    }
                    totalBottomLayerMoves += analyzed;
                    this->board.revert();
                    curScore = -curScore;
                }

                // Is this the new top score? If so, save the move
                if (std::isnan(bestScore) || curScore > bestScore) {
                    bestScore = curScore;
                    bestMove = move;
                }

            });
        }
    }

    if (std::isnan(bestScore)) {       // no legal moves
        bestScore = this->board.isCheck() ? -10000 + random(rng) : 0;
    }


    if (!cacheName.isInvalid()) {
        auto value = std::make_tuple(bestScore, depth, totalBottomLayerMoves, bestMove);
        auto result = memoizedPositions.insert(std::make_pair(cacheName, value));
        if (!result.second) { result.first->second = value; }
    }

    return ChessEngine::CalculatedMove(bestMove, bestScore, totalBottomLayerMoves, depth);
}




std::optional<std::tuple<float, int, int, BoardMove>> TannerEngine::getMemoization() const {
    auto cacheName = board.getUniqueCacheName();
    if (cacheName.isInvalid()) return std::nullopt;
    auto cacheEntry = memoizedPositions.find(cacheName);
    if (cacheEntry == memoizedPositions.end()) return std::nullopt;
    return {cacheEntry->second};
}

int TannerEngine::getMemoizationCount() const {
    return memoizedPositions.size();
}


void TannerEngine::resetMemoizations() {
    memoizedPositions = std::unordered_map<ChessBoard::CacheName, std::tuple<float, int, int, BoardMove>>();
}


