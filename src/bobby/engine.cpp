#include "engine.h"


ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

int ChessEngine::cacheCalls[33] = {0};
int ChessEngine::cacheHits[33] = {0};

std::tuple<float, int, BoardMove> ChessEngine::findBestMove() {
    int depth = 5;
    while (true) {
        std::tuple<float, int, BoardMove> tup = this->findBestMove(depth);
        float score = std::get<0>(tup);
        int bottomLayerMoves = std::get<1>(tup);
        if (score > 1000 || depth >= 30 || bottomLayerMoves >= 600000) return tup;
        depth += 1;
    }
}

std::tuple<float, int, BoardMove> ChessEngine::findBestMove(int depth) {
    static std::default_random_engine rng(133742);
    static std::normal_distribution<float> random(0.0, 0.00002);

    // Find the best of all 64*64 theoretical moves
    float bestScore = NAN;
    BoardMove bestMove = "e2e4";
    int totalBottomLayerMoves = 0;

    // Our move is from position (i, j) to (k, l)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.squares[i][j].color() != board.curColor) continue;
            board.forEachMove(BoardPosition(i, j), [this, depth, &bestScore, &bestMove, &totalBottomLayerMoves] (BoardMove move) {
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
                            curScore *= std::pow(0.999, depth - cacheEntry->second.second);
                            nd = false;
                        }
                    }
                    if (nd) {
                        // Calculate board score
                        if (depth > 1) {            // recurse deeper
                            int n;
                            std::tie(curScore, n, std::ignore) = this->findBestMove(depth - 1);
                            curScore *= 0.999;
                            totalBottomLayerMoves += n;
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

    return std::make_tuple(bestScore, totalBottomLayerMoves, bestMove);
}





int ChessEngine::getMemoizationCount() const {
    return memoizedPositions.size();
}


void ChessEngine::resetMemoizations() {
    memoizedPositions = std::unordered_map<ChessBoard::CacheName, std::pair<float, int>>();
}


