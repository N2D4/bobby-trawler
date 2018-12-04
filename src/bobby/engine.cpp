#include "engine.h"



ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

BoardMove ChessEngine::findBestMove() {
    // TODO
    while (true) {
        BoardPosition from = BoardPosition(rand() % 8, rand() % 8);
        BoardPosition to = BoardPosition(rand() % 8, rand() % 8);
        BoardMove move = BoardMove(from, to);
        if (this->board.isLegal(move)) return move;
    }
}