#ifndef BOBBY_TRAWLER_BOBBY_WASM_H_
#define BOBBY_TRAWLER_BOBBY_WASM_H_

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>
#include "emscripten.h"
#include "engines/engine.h"
#include "engines/tanner.h"
#include "engines/daedrian.h"
#include "board/chessboard.h"
#include "board/boardmoves.h"
#include "board/boardsquare.cpp"


extern "C" {
    void move(int fromRow, int fromCol, int toRow, int toCol);
    int askTanner();
}


#endif  // BOBBY_TRAWLER_BOBBY_WASM_H_
