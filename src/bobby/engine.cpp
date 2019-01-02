#include "engine.h"


ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

std::tuple<float, int, BoardMove> ChessEngine::findBestMove() {
    int depth = 4;
    while (true) {
        std::tuple<float, int, BoardMove> tup = this->findBestMove(depth);
        float score = std::get<0>(tup);
        int bottomLayerMoves = std::get<1>(tup);
        if (score > 1000 || depth >= 30 || bottomLayerMoves >= 100000) return tup;
        depth += 1;
    }
}

std::tuple<float, int, BoardMove> ChessEngine::findBestMove(int depth) {
    static std::default_random_engine rng(1337);
    static std::normal_distribution<float> random(0.0, 0.00002);

    // Find the best of all 64*64 theoretical moves
    float bestScore = NAN;
    BoardMove bestMove = "e2e4";
    int totalBottomLayerMoves = 0;

    // Our move is from position (i, j) to (k, l)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.squares[i][j].color() != board.curColor) continue; // exit early
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    BoardMove move = BoardMove(BoardPosition(i, j), BoardPosition(k, l));
                    float curScore = 0;

                    // If it's not legal, continue with the next move
                    if (!this->board.isLegal(move)) continue;


                    totalBottomLayerMoves += 1;
                    this->board.move(move);
                    {
                        long long cacheName = board.getUniqueCacheName();
                        std::unordered_map<long long, std::pair<float, int>>::iterator cacheEntry = memoizedPositions.find(cacheName);
                        if (cacheName != 0 && cacheEntry != memoizedPositions.end() && cacheEntry->second.second >= depth) {
                            curScore = cacheEntry->second.first;
                            curScore *= std::pow(0.999, depth - cacheEntry->second.second);
                        } else {
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

                            if (cacheName != 0) {
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
                }
            }
        }
    }

    if (std::isnan(bestScore)) {       // no legal moves
        bestScore = this->board.isCheck() ? -10000 + random(rng) : 0;
    }

    return std::make_tuple(bestScore, totalBottomLayerMoves, bestMove);
}





int ChessEngine::getMemoizationCount() {
    return memoizedPositions.size();
}


void ChessEngine::resetMemoizations() {
    memoizedPositions = std::unordered_map<long long, std::pair<float, int>>();
}


