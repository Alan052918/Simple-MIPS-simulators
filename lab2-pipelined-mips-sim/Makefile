CFLAGS=-g -std=c++11

# target directory
BIN=bin

$(BIN)/mips : src/MIPS_pipeline.cpp
	@echo "mkdir -p $(BIN)"; mkdir -p $(BIN)
	@echo "g++ $^ $(CFLAGS) -o $@"; g++ $^ $(CFLAGS) -o $@

.PHONY: clean
clean :
	@echo "rm -rf $(BIN)"; rm -rf $(BIN)
