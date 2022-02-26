SHELL = /bin/bash

CPPC = g++
EMC = em++
CPPFLAGS = -g -I src/bobby -std=c++17 -O3

SRC_DIR := src/bobby
OBJ_DIR := out/obj
WASM_OBJ_DIR := out/wasm
CPP_FILES := $(shell find $(SRC_DIR) -type f -name *.cpp)
H_FILES := $(shell find $(SRC_DIR) -type f -name *.h)
OBJ_SRC_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/src/%.o,$(CPP_FILES))
WASM_OBJ_SRC_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(WASM_OBJ_DIR)/src/%.o,$(CPP_FILES))

.DELETE_ON_ERROR:




out/BobbyTrawler: $(OBJ_SRC_FILES) $(OBJ_DIR)/bobby-trawler.o
	@mkdir -p $(@D)
	$(CPPC) -O3 $^ -o $@

$(OBJ_DIR)/bobby-trawler.o: src/bobby-trawler.cpp src/bobby-trawler.h $(H_FILES)
	@mkdir -p $(@D)
	$(CPPC) -c -o $@ $< $(CPPFLAGS)

$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp $(H_FILES)
	@mkdir -p $(@D)
	$(CPPC) -c -o $@ $< $(CPPFLAGS)




out/BobbyTrawler.js: $(WASM_OBJ_SRC_FILES) $(WASM_OBJ_DIR)/bobby-wasm.o
	@mkdir -p $(@D)
	$(EMC) -O3 $^ -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s ALLOW_MEMORY_GROWTH=1  -o $@

$(WASM_OBJ_DIR)/bobby-wasm.o: src/bobby-wasm.cpp src/bobby-wasm.h $(H_FILES)
	@mkdir -p $(@D)
	$(EMC) -c -o $@ $< $(CPPFLAGS)

$(WASM_OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp $(H_FILES)
	@mkdir -p $(@D)
	$(EMC) -c -o $@ $< $(CPPFLAGS)




lib/catch.hpp:
	@mkdir -p $(@D)
	@echo "Couldn't find lib/catch.hpp. Downloading Catch2... (expected version: v2.5.0)"
	@wget -O lib/catch.hpp -t 5 "https://raw.githubusercontent.com/catchorg/Catch2/6ccd467094973824d89efb16cbc553e279f79823/single_include/catch2/catch.hpp"

.PHONY: clean
clean:
	-rm -rf out
	-rm -rf lib/catch.hpp
