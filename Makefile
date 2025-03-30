WORK_DIR  = $(shell pwd)
SRC_DIR = $(WORK_DIR)/src
HEADER_DIR = $(WORK_DIR)/include
BUILD_DIR = $(WORK_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj
BIN = $(BUILD_DIR)/nemu_cpp
NPROC := $(shell nproc)

# Spike Variables
SPIKE_SRC_DIR = $(WORK_DIR)/spike
SPIKE_BUILD_DIR = $(BUILD_DIR)/spike_build
SPIKE_MAKEFILE = $(SPIKE_BUILD_DIR)/Makefile
SPIKE_LIBS = libspike_main.a libriscv.a libdisasm.a libsoftfloat.a libfesvr.a libfdt.a
SPIKE_INC_LIBS = $(addprefix $(SPIKE_BUILD_DIR)/, $(SPIKE_LIBS))
SPIKE_IMG = $(BUILD_DIR)/riscv32-spikes-so

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CXX_FLAGS := -std=c++20 -I$(HEADER_DIR) -fpermissive

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

#########
# Spike
#########
.PHONY: spike
spike: $(SPIKE_MAKEFILE)
	$(MAKE) -C $(SPIKE_BUILD_DIR) O=$(SPIKE_BUILD_DIR) -j $(NPROC) \
	CXXFLAGS="-fvisibility=hidden"
	$(CXX) -std=c++20 -O2 -shared -fPIC -fvisibility=hidden $(SPIKE_INC_LIBS) -o $(SPIKE_IMG)

$(SPIKE_MAKEFILE):
	mkdir -p $(@D)
	cd $(SPIKE_BUILD_DIR) && $(SPIKE_SRC_DIR)/configure

.PHONY: run
run:
	@cd $(BUILD_DIR) && ./$(notdir $(BIN))

.PHONY: clean spike-clean
clean:
	rm -rf $(BUILD_DIR)

spike-clean:
	rm -rf $(SPIKE_BUILD_DIR)
	rm -rf $(SPIKE_IMG)
