#include "engine.h"



ChessEngine::ChessEngine(ChessBoard& board) : board(board) { }

DetailedMove ChessEngine::findBestMove() {
    // TODO
    while (true) {
        BoardPosition from = BoardPosition(rand() % 8, rand() % 8);
        BoardPosition to = BoardPosition(rand() % 8, rand() % 8);
        DetailedMove move = BoardMove(from, to).detailed(this->board);
        if (this->board.isLegal(move)) return move;
    }
}