# main compiler
CC=g++
CFLAGS=-g -std=c++11

# target directory
BIN=bin

# MIPS component definition configuration
SRC_DIR=src/MIPS
OBJ_DIR=obj
SRC_FILES:=$(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES:=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRC_FILES))

# make MIPS
$(BIN)/MIPS : $(OBJ_FILES) src/main.cc
	@echo "Linking MIPS..."
	@echo "  mkdir -p $(BIN)"; mkdir -p $(BIN)
	@echo "  $(CC) $^ $(CFLAGS) -o $@"; $(CC) $^ $(CFLAGS) -o $@
	@echo "Success! Removing intermediate object files"
	@echo "  rm -rf $(OBJ_DIR)"; rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	@echo "Compiling MIPS components..."
	@echo "  mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR)
	@echo "  $(CC) $< $(CFLAGS) -c -o $@"; $(CC) $< $(FLAGS) -c -o $@

.PHONY: clean
clean :
	@echo "Cleaning..."
	@echo "  rm -rf $(BIN)"; rm -rf $(BIN)
	@echo "  rm -rf $(OBJ_DIR)"; rm -rf $(OBJ_DIR)
