# Compiler settings
CXX = g++
CXXFLAGS = -Wall -g -Isrc

# Directories
SRC = src
TEST_DIR = tests

# Dependencies
VM_SRC = $(SRC)/vm/bvm.cpp $(SRC)/gc/gc.cpp

# --- Targets ---

# Default target builds VM, Assembler, and runs standard tests
all: vm assembler run

# 1. Standard Components
vm: $(SRC)/vm/bvm_main.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o vm $(SRC)/vm/bvm_main.cpp $(VM_SRC)

assembler: $(SRC)/assembler/assembly_main.cpp $(SRC)/assembler/assembler.cpp
	$(CXX) $(CXXFLAGS) -o assembler $(SRC)/assembler/assembly_main.cpp $(SRC)/assembler/assembler.cpp

# 2. Compile Individual GC Tests
test_1: $(TEST_DIR)/test_1.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_1 $(TEST_DIR)/test_1.cpp $(VM_SRC)

test_unreachable: $(TEST_DIR)/test_gc_unreachable.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_unreachable $(TEST_DIR)/test_gc_unreachable.cpp $(VM_SRC)

test_transitive: $(TEST_DIR)/test_transitive.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_transitive $(TEST_DIR)/test_transitive.cpp $(VM_SRC)

test_cycles: $(TEST_DIR)/test_gc_cycles.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_cycles $(TEST_DIR)/test_gc_cycles.cpp $(VM_SRC)

test_deep: $(TEST_DIR)/test_deep_object_graph.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_deep $(TEST_DIR)/test_deep_object_graph.cpp $(VM_SRC)

test_closure: $(TEST_DIR)/test_closure_capture.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_closure $(TEST_DIR)/test_closure_capture.cpp $(VM_SRC)

test_stress: $(TEST_DIR)/test_stress_allocation.cpp $(VM_SRC)
	$(CXX) $(CXXFLAGS) -o test_stress $(TEST_DIR)/test_stress_allocation.cpp $(VM_SRC)

# 3. Group Compilation (Internal Helper)
compile_tests: test_1 test_unreachable test_transitive test_cycles test_deep test_closure test_stress

# 4. TARGET: Run All Tests (Correctness Check)
run: compile_tests
	@echo "========================================"
	@echo "       RUNNING ALL GC TESTS             "
	@echo "========================================"
	@echo "[1/7] Basic Reachability:"
	@./test_1
	@echo "----------------------------------------"
	@echo "[2/7] Unreachable Objects:"
	@./test_unreachable
	@echo "----------------------------------------"
	@echo "[3/7] Transitive Reachability:"
	@./test_transitive
	@echo "----------------------------------------"
	@echo "[4/7] Cyclic References:"
	@./test_cycles
	@echo "----------------------------------------"
	@echo "[5/7] Deep Object Graph:"
	@./test_deep
	@echo "----------------------------------------"
	@echo "[6/7] Closure Capture:"
	@./test_closure
	@echo "----------------------------------------"
	@echo "[7/7] Stress Allocation:"
	@./test_stress
	@echo "========================================"
	@echo "       ALL TESTS COMPLETED              "
	@echo "========================================"

# 5. TARGET: Benchmark (Timing & Reporting)
# Requires run_benchmark.sh script to exist
benchmark_gc: compile_tests
	@chmod +x run_benchmark.sh
	@./run_benchmark.sh

clean:
	rm -f vm assembler test_1 test_unreachable test_transitive test_cycles test_deep test_closure test_stress *.o *.bin