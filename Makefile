WORK_DIR  = $(shell pwd)
SRC_DIR = $(WORK_DIR)/src
HEADER_DIR = $(WORK_DIR)/include
BUILD_DIR = $(WORK_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj
BIN = $(BUILD_DIR)/nemu_cpp

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CXX_FLAGS := -std=c++20 -I$(HEADER_DIR)

.PHONY: all
all: build

.PHONY: build
build: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

.PHONY: run
run:
	@cd $(BUILD_DIR) && ./$(notdir $(BIN))

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)