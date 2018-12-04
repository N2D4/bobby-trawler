CPPC = g++
CPPFLAGS = -I src/bobby -std=c++11 -O3

SRC_DIR := src
OBJ_DIR := out/obj
CPP_FILES := $(shell find $(SRC_DIR) -type f -name *.cpp)
H_FILES := $(shell find $(SRC_DIR) -type f -name *.h)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))


out/BobbyTrawler: $(OBJ_FILES)
	$(CPPC) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(H_FILES)
	@mkdir -p $(@D)
	$(CPPC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -rf out
