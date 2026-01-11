# Compiler settings
CXX = g++
CXXFLAGS = -Wall -g -Isrc

# Define the Source Directory
SRC = src

# Targets
all: vm assembler benchmark

# 1. Compile VM
vm: $(SRC)/vm/bvm_main.cpp $(SRC)/vm/bvm.cpp
	$(CXX) $(CXXFLAGS) -o vm $(SRC)/vm/bvm_main.cpp $(SRC)/vm/bvm.cpp

# 2. Compile Assembler
assembler: $(SRC)/assembler/assembly_main.cpp $(SRC)/assembler/assembler.cpp
	$(CXX) $(CXXFLAGS) -o assembler $(SRC)/assembler/assembly_main.cpp $(SRC)/assembler/assembler.cpp

# 3. Compile Benchmark Tool
benchmark: $(SRC)/benchmark_main.cpp $(SRC)/vm/bvm.cpp $(SRC)/assembler/assembler.cpp
	$(CXX) $(CXXFLAGS) -o benchmark $(SRC)/benchmark_main.cpp $(SRC)/vm/bvm.cpp $(SRC)/assembler/assembler.cpp

# 4. Run Everything (Dynamic Test Suite)
run: all
	@echo "========================================"
	@echo "       RUNNING ALL TEST CASES           "
	@echo "========================================"
	@# This loop finds every .asm file in tests/ and runs it
	@for asm_file in tests/*.asm; do \
		echo "----------------------------------------"; \
		echo "Testing: $$asm_file"; \
		bin_file=$${asm_file%.asm}.bin; \
		./assembler $$asm_file $$bin_file; \
		if [ $$? -eq 0 ]; then \
			./vm $$bin_file; \
		else \
			echo "Assembler Failed for $$asm_file"; \
		fi \
	done
	

# Clean up binaries
clean:
	rm -f vm assembler benchmark *.bin tests/*.bin