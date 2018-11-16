#include <vector>
#include "chessboard.cpp"
using namespace std;




class ChessEngine {
    ChessBoard board;

    void startNewGame() {
        board = ChessBoard();
    }

    ChessMove findBestMove() {
        // TODO
        return ChessMove();
    }

};