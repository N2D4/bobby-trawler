CPPC = g++
CPPFLAGS = -I src/bobby -std=c++11

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

.PHONY: list
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | xargs

.PHONY: debug
debug:
	@echo $(H_FILES)