SHELL = /bin/bash

CPPC = g++
CPPFLAGS = -I src/bobby -std=c++17 -O3

SRC_DIR := src
OBJ_DIR := out/obj
CPP_FILES := $(shell find $(SRC_DIR) -type f -name *.cpp)
H_FILES := $(shell find $(SRC_DIR) -type f -name *.h)
OBJ_SRC_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/src/%.o,$(CPP_FILES))

.DELETE_ON_ERROR:

out/BobbyTrawler: $(OBJ_SRC_FILES)
	$(CPPC) $^ -o $@


$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp $(H_FILES)
	@mkdir -p $(@D)
	$(CPPC) -c -o $@ $< $(CPPFLAGS)

lib/catch.hpp:
	@mkdir -p $(@D)
	@echo "Couldn't find lib/catch.hpp. Downloading Catch2... (expected version: v2.5.0)"
	@wget -O lib/catch.hpp -t 5 "https://raw.githubusercontent.com/catchorg/Catch2/6ccd467094973824d89efb16cbc553e279f79823/single_include/catch2/catch.hpp"

.PHONY: clean
clean:
	-rm -rf out
	-rm -rf lib/catch.hpp
