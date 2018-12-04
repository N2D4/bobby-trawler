#include "engine.h"



ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

BoardMove ChessEngine::findBestMove() {
    static std::default_random_engine rng;
    static std::normal_distribution<float> random(0.0, 0.2);

    // Find the best of all 64*64 theoretical moves
    float bestScore = -1000;
    BoardMove bestMove = "e2e4";

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    BoardMove move = BoardMove(BoardPosition(i, j), BoardPosition(k, l));
                    if (!this->board.isLegal(move)) continue;

                    this->board.move(move);

                        // Calculate board score
                        float curScore = 0;
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

                        if (curScore >= bestScore) {
                            bestScore = curScore;
                            bestMove = move;
                        }

                    this->board.revert();
                }
            }
        }
    }
    
    return bestMove;
}