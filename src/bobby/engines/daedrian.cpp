#include "daedrian.h"



DaedrianEngine::DaedrianEngine(ChessBoard& board) : board(board) { }

int DaedrianEngine::cacheCalls[33] = {0};
int DaedrianEngine::cacheHits[33] = {0};

ChessEngine::CalculatedMove DaedrianEngine::findBestMove() {
    int b = 0;
    const float seldec = -0.25;
    while (true) {
        ChessEngine::CalculatedMove tup = findBestMove(4 + 2*b, seldec, false, 1, 1, 6);
        if (std::abs(tup.score) > 1000 || b >= 30 || tup.movesAnalyzed >= 2000000) return tup;
        b += 1;
    }
}

ChessEngine::CalculatedMove DaedrianEngine::findBestMove(float selwidth, float seldec, bool ignoreWidth, int depth, int nextDepth, int remSelDepth) {
    static std::default_random_engine rng(133742);
    static std::normal_distribution<float> random(0.0, 0.00002);

    int selw = std::max(1, (int) std::round(selwidth));
    long totalBottomLayerMoves = 0;

    std::vector<std::pair<float, BoardMove>> candidates = findCandidates(depth, ignoreWidth ? 9999999 : (remSelDepth <= 1 ? 1 : selw), totalBottomLayerMoves);


    if (candidates.size() == 1) {
        return ChessEngine::CalculatedMove(candidates[0].second, candidates[0].first, totalBottomLayerMoves, depth, depth);
    }

    std::tuple<float, int, BoardMove> best = std::make_tuple(- std::numeric_limits<float>::infinity(), 0, "h7h7");
    for (std::pair<float, BoardMove> pair : candidates) {
        board.move(pair.second);
            ChessEngine::CalculatedMove nxt = findBestMove(selwidth - seldec, seldec, !ignoreWidth, nextDepth, nextDepth, remSelDepth - 1);
        board.revert();
        totalBottomLayerMoves += nxt.movesAnalyzed;
        if (-nxt.score > std::get<0>(best)) {
            best = std::make_tuple(-nxt.score, nxt.selectiveDepth, pair.second);
        }
    }

    if (std::isinf(std::get<0>(best))) std::get<0>(best) = board.isCheck() ? -10000 + random(rng) : 0;

    return ChessEngine::CalculatedMove(std::get<2>(best), std::get<0>(best), totalBottomLayerMoves, depth, std::get<1>(best) + 1);
}



std::vector<std::pair<float, BoardMove>> DaedrianEngine::findCandidates(int depth, int count, long& totalBottomLayerMoves) {
    static std::default_random_engine rng(133742);
    static std::normal_distribution<float> random(0.0, 0.000002);

    std::vector<std::pair<float, BoardMove>> candidates;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.squares[i][j].color() != board.curColor) continue;
            board.forEachMove(BoardPosition(i, j), [this, depth, count, &candidates, &totalBottomLayerMoves] (BoardMove move) {
                float curScore = 0;
                totalBottomLayerMoves += 1;
                this->board.move(move);
                {
                    bool nd = true;
                    // Check for caching
                    ChessBoard::CacheName cacheName;
                    if (depth > 1) {
                        cacheName = board.getUniqueCacheName();
                    }
                    if (!cacheName.isInvalid()) {
                        int pieceCount = board.getPieceCount();
                        if (pieceCount <= 32) {
                            cacheCalls[pieceCount]++;
                        }
                        std::unordered_map<ChessBoard::CacheName, std::pair<float, int>>::iterator cacheEntry = memoizedPositions.find(cacheName);
                        if (cacheEntry != memoizedPositions.end() && cacheEntry->second.second >= depth) {
                            // We found a cache entry!
                            if (pieceCount <= 32) {
                                cacheHits[pieceCount]++;
                            }
                            curScore = cacheEntry->second.first;
                            if (curScore >= 1000) curScore *= std::pow(0.999, depth - cacheEntry->second.second);
                            nd = false;
                        }
                    }
                    if (nd) {
                        // Calculate board score
                        if (depth > 1) {            // recurse deeper
                            int n;
                            std::pair<float, BoardMove> mov = this->findCandidates(depth - 1, 1, totalBottomLayerMoves)[0];
                            curScore = mov.first;
                            if (std::abs(curScore) > 1000) curScore *= 0.999;
                        } else {                    // base case
                            curScore = this->board.getBoardScore(this->board.curColor);

                            // Add some randomness so our computer isn't 100% deterministic
                            curScore += random(rng);
                        }

                        if (!cacheName.isInvalid()) {
                            memoizedPositions[cacheName] = std::make_pair(curScore, depth);
                        }
                    }
                    this->board.revert();
                    curScore = -curScore;
                }

                candidates.push_back(std::make_pair(curScore, move));

            });
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const std::pair<float, BoardMove>& a, std::pair<float, BoardMove>& b) -> bool {
        return a.first > b.first;
    });

    auto res = std::vector(candidates.begin(), candidates.begin() + std::min((size_t) count, candidates.size()));

    return res;

}





int DaedrianEngine::getMemoizationCount() const {
    return memoizedPositions.size();
}


void DaedrianEngine::resetMemoizations() {
    memoizedPositions = std::unordered_map<ChessBoard::CacheName, std::pair<float, int>>();
}


