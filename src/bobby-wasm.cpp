#include "bobby-wasm.h"

ChessBoard board;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void move(int fromCol, int fromRow, int toCol, int toRow) {
        board.move(BoardMove(BoardPosition(fromCol, fromRow), BoardPosition(toCol, toRow)));
    }

    EMSCRIPTEN_KEEPALIVE
    int askTanner(int maxCalculationEffort) {
        TannerEngine tanner(board);
        auto move = tanner.findBestMoveWithMaxEffort(maxCalculationEffort).move;
        return ((move.from.column * 8 + move.from.row) * 8 + move.to.column) * 8 + move.to.row;
    }

    EMSCRIPTEN_KEEPALIVE
    void newGame() {
        board = ChessBoard();
    }
}
