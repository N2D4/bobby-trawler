#include "engine.h"



void ChessEngine::startNewGame() {
    board = ChessBoard();
}

BoardMove ChessEngine::findBestMove() {
    // TODO
    return BoardMove("e2", "e4");
}