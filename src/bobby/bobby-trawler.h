#ifndef BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_
#define BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>
#include "engines/engine.h"
#include "engines/tanner.h"
#include "board/chessboard.h"
#include "board/boardmoves.h"
#include "board/boardsquare.cpp"


/**
 * Reads standard input and halts the program until it receives the command passed to this function in standard input.
 */
std::string waitforcmd(std::string str);

/**
 * Reads standard input and halts the program until it receives the command passed to this function in standard input.
 */
std::string waitforcmd(std::vector<std::string> v);

/**
 * A function for reading a single command (space-separated array of strings) from standard input.
 */
std::vector<std::string> readcmd();

/**
 * Finds the first occurence of any of the elements in find, and returns the range (inclusive from, exclusive to).
 */
std::pair<size_t, size_t> findany(std::string s, std::vector<std::string> find, size_t start);

/**
 * Splits the string by whitespaces, removing empty elements.
 */
std::vector<std::string> nonEmptySplits(std::string str);


#endif  // BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_