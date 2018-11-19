#ifndef BOBBY_TRAWLER_BOBBY_ENGINE_H_
#define BOBBY_TRAWLER_BOBBY_ENGINE_H_

#include <vector>
#include "chessboard.h"

class ChessEngine {
    ChessBoard board;
    void startNewGame();
    BoardMove findBestMove();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_