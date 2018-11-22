#ifndef BOBBY_TRAWLER_BOBBY_ENGINE_H_
#define BOBBY_TRAWLER_BOBBY_ENGINE_H_

#include <vector>
#include <stdlib.h>
#include "board/chessboard.h"

class ChessEngine {
    private:
        ChessBoard& board;

    public:
        ChessEngine(ChessBoard& board);

        DetailedMove findBestMove();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_