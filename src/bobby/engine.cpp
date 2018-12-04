#include "engine.h"



ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

BoardMove ChessEngine::findBestMove() {
    return this->findBestMove(4).second;
}

std::pair<float, BoardMove> ChessEngine::findBestMove(int depth) {
    static std::default_random_engine rng;
    static std::normal_distribution<float> random(0.0, 0.0002);

    // Find the best of all 64*64 theoretical moves
    float bestScore = NAN;
    BoardMove bestMove = "e2e4";

    // Our move is from position (i, j) to (k, l)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    BoardMove move = BoardMove(BoardPosition(i, j), BoardPosition(k, l));

                    // If it's not legal, continue with the next move
                    if (!this->board.isLegal(move)) continue;

                    this->board.move(move);

                        // Calculate board score
                        float curScore = 0;
                        if (depth > 1) {            // recurse deeper
                            curScore = - this->findBestMove(depth - 1).first;
                        } else {                    // base case
                            for (int m = 0; m < 8; m++) {
                                for (int n = 0; n < 8; n++) {
                                    BoardSquare sq = this->board.squares[m][n];
                                    int score = sq.type() == BoardSquare::Type::PAWN ? 1
                                              : sq.type() == BoardSquare::Type::KNIGHT ? 3
                                              : sq.type() == BoardSquare::Type::BISHOP ? 3
                                              : sq.type() == BoardSquare::Type::ROOK ? 5
                                              : sq.type() == BoardSquare::Type::QUEEN ? 9
                                              : 0;
                                    if (sq.color() == this->board.curColor) score *= -1;
                                    curScore += score;
                                }
                            }

                            // Add some randomness so our computer isn't 100% deterministic
                            curScore += random(rng);
                        }

                    this->board.revert();

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
        bestScore = this->board.isCheck() ? -1000 : 0;
    }

    return std::pair<float, BoardMove>(bestScore, bestMove);
}