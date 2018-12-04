#ifndef BOBBY_TRAWLER_BOBBY_ENGINE_H_
#define BOBBY_TRAWLER_BOBBY_ENGINE_H_

#include <vector>
#include <stdlib.h>
#include <random>
#include "board/chessboard.h"

/**
 * The class containing the code for the Bobby Trawler chess engine itself.
 */
class ChessEngine {
    private:
        ChessBoard& board;

    public:
        ChessEngine(ChessBoard& board);

        BoardMove findBestMove();
};

#endif  // BOBBY_TRAWLER_BOBBY_ENGINE_H_