#include "bobby-wasm.h"

ChessBoard board;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void move(int fromCol, int fromRow, int toCol, int toRow) {
        board.move(BoardMove(BoardPosition(fromCol, fromRow), BoardPosition(toCol, toRow)));
    }

    EMSCRIPTEN_KEEPALIVE
    int askTanner() {
        TannerEngine tanner(board);
        auto move = tanner.findBestMove().move;
        return ((move.from.column * 8 + move.from.row) * 8 + move.to.column) * 8 + move.to.row;
    }
}
