#include "daedrian.h"



DaedrianEngine::DaedrianEngine(ChessBoard& board) : board(board) { }

int DaedrianEngine::cacheCalls[33] = {0};
int DaedrianEngine::cacheHits[33] = {0};

ChessEngine::CalculatedMove DaedrianEngine::findBestMove() {
    return findBestMove(5, 2.5, 1, 1);
}

ChessEngine::CalculatedMove DaedrianEngine::findBestMove(float selwidth, float seldec, int depth, int nextDepth) {
    static std::default_random_engine rng(133742);
    static std::normal_distribution<float> random(0.0, 0.00002);

    int selw = std::max(1, (int) std::round(selwidth));
    long totalBottomLayerMoves = 0;

    std::vector<std::pair<float, BoardMove>> candidates = findCandidates(depth, selw, totalBottomLayerMoves);


    if (candidates.size() == 1) {
        return ChessEngine::CalculatedMove(candidates[0].second, candidates[0].first, totalBottomLayerMoves, depth, depth);
    }

    std::tuple<float, int, BoardMove> best = std::make_tuple(- std::numeric_limits<float>::infinity(), 0, "h7h7");
    for (std::pair<float, BoardMove> pair : candidates) {
        //std::cout << " " << pair.first << " " << std::string(pair.second) << std::endl;
        board.move(pair.second);
            ChessEngine::CalculatedMove nxt = findBestMove(selwidth - seldec, seldec, nextDepth, nextDepth);
        board.revert();
        totalBottomLayerMoves += nxt.movesAnalyzed;
        if (-nxt.score > std::get<0>(best)) {
            best = std::make_tuple(-nxt.score, nxt.selectiveDepth, pair.second);
        }
    }

    if (std::isinf(std::get<0>(best))) std::get<0>(best) = board.isCheck() ? -10000 + random(rng) : 0;

    //std::cout << selwidth << " " << std::get<0>(best) << " " << std::string(std::get<2>(best)) << " " << std::get<1>(best) << std::endl;
    return ChessEngine::CalculatedMove(std::get<2>(best), std::get<0>(best), totalBottomLayerMoves, depth, std::get<1>(best) + 1);
}



std::vector<std::pair<float, BoardMove>> DaedrianEngine::findCandidates(int depth, int count, long& totalBottomLayerMoves) {
    static std::default_random_engine rng(133742);
    static std::normal_distribution<float> random(0.0, 0.00002);

    std::vector<std::pair<float, BoardMove>> candidates;
    candidates.reserve(count);

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
                            curScore = this->board.getMaterialScore(this->board.curColor);

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

                // Is this a new top score? If so, save the move
                if (candidates.size() < count) {
                    candidates.push_back(std::make_pair(- std::numeric_limits<float>::infinity(), BoardMove("h7h7")));
                }
                if (curScore >= candidates.back().first) {
                    if (candidates.size() == 1) {
                        candidates[0] = std::make_pair(curScore, move);
                    }
                    for (int i = candidates.size() - 2; i >= 0; i--) {
                        if (curScore < candidates[i].first) {
                            candidates[i+1] = std::make_pair(curScore, move);
                            break;
                        }
                        candidates[i+1] = candidates[i];
                        if (i == 0) {
                            candidates[0] = std::make_pair(curScore, move);
                        }
                    }
                }
                

            });
        }
    }

    return candidates;

}





int DaedrianEngine::getMemoizationCount() const {
    return memoizedPositions.size();
}


void DaedrianEngine::resetMemoizations() {
    memoizedPositions = std::unordered_map<ChessBoard::CacheName, std::pair<float, int>>();
}


