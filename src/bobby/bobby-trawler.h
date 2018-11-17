#ifndef BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_
#define BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_

#include <iostream>
#include <vector>
#include "engine.h"


// function declarations
std::string waitforcmd(std::string str);
std::string waitforcmd(std::vector<std::string> v);
std::vector<std::string> readcmd();
std::pair<size_t, size_t> findany(std::string s, std::vector<std::string> find, size_t start);
std::vector<std::string> nonEmptySplits(std::string str);

#endif  // BOBBY_TRAWLER_BOBBY_BOBBY_TRAWLER_H_