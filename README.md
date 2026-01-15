# Mark-Sweep Garbage Collector for Bytecode Virtual Machine

This project implements a **Mark-Sweep Garbage Collector (GC)** integrated into a custom Bytecode Virtual Machine (BVM). It demonstrates dynamic memory management, automatic garbage collection, and heap handling in C++.

The implementation focuses on the core mechanics of the Mark-Sweep algorithm: identifying reachable objects ("Mark") and reclaiming unused memory ("Sweep") to prevent memory leaks and handle complex object graphs, including cyclic references.

## 🚀 Features

- **Mark-Sweep Algorithm**:
  - **Mark Phase**: Recursively traverses object graphs starting from "roots" (VM stack) to tag live objects.
  - **Sweep Phase**: Iterates through the heap to reclaim unmarked objects and return them to the free list.
- **Heap Management**:
  - Fixed-size heap (120,000 slots) managed via a specific `Object` structure.
  - Efficient "Free List" allocation strategy.
- **Cycle Detection**: Capable of collecting cyclic data structures (e.g., A references B, B references A) which reference counting cannot handle.
- **Deep Graph Traversal**: Robust recursion handling for deep object graphs (linked lists, nested closures).
- **Performance Benchmarking**: Includes a script and test suite to measure execution time and object reclamation counts.

## 📂 Project Structure

```text
├── src/
│   ├── gc/
│   │   ├── gc.h            # GC definitions and Object struct
│   │   └── gc.cpp          # Mark-Sweep implementation
│   ├── vm/
│   │   ├── bvm.h           # VM definition (Heap/Stack)
│   │   └── bvm.cpp         # VM core logic
│   ├── assembler/          # Assembler for converting .asm to bytecode
│   └── commons.h           # Shared OpCodes
├── tests/                  # C++ Test Suite for GC verification
│   ├── test_1.cpp                  # Basic Reachability
│   ├── test_gc_unreachable.cpp     # Unreachable Object Collection
│   ├── test_transitive.cpp         # Transitive Reachability
│   ├── test_gc_cycles.cpp          # Cyclic Reference Handling
│   ├── test_deep_object_graph.cpp  # Deep Object Graphs
│   ├── test_closure_capture.cpp    # Closure/Environment Survival
│   └── test_stress_allocation.cpp  # Stress Testing
├── makefile                # Build configuration
└── run_benchmark.sh        # Automated benchmarking script
```

## 🛠️ Building and Running

This project uses make for compilation and testing.

### Prerequisites

- C++ Compiler (g++ recommended)
- Make utility

### Build Commands

```bash
# Compile the VM, Assembler, and all GC tests
make all

# Clean build artifacts
make clean
```

### Running Tests

To verify the correctness of the Garbage Collector, run the full test suite. This will compile and execute all 7 test cases defined in the tests/ directory.

```bash
make run
```

**Expected Output:**

```text
[1/7] Basic Reachability:
... [PASSED] Object 'a' survived.
[2/7] Unreachable Objects:
... [PASSED] Object 'a' was correctly freed.
...
[7/7] Stress Allocation:
... [PASSED] Heap empty after final GC.
```

## 📊 Performance Benchmark

To measure the performance of the GC under various conditions (Time vs. Objects Freed), use the provided benchmark script:

```bash
make benchmark_gc
```

This will generate a colored report in the terminal:

```text
TEST CASE                 | RESULT     | TIME (s)        | FREED (objs)   
--------------------------+------------+-----------------+----------------
Basic Reachability        | PASSED     | 0.000890        | 0              
Unreachable Objects       | PASSED     | 0.000953        | 1              
Transitive Reachability   | PASSED     | 0.000887        | 0              
Cyclic References         | PASSED     | 0.000858        | 0   
Deep Object Graph         | PASSED     | 0.000888        | 0              
Closure Capture           | PASSED     | 0.000853        | 0              
Stress Allocation         | PASSED     | 0.002200        | 300000           
```

## 🧪 Test Cases Explained

The tests/ folder contains specific scenarios designed to prove the robustness of the GC:

| Test File | Description | Goal |
|-----------|-------------|------|
| `test_1.cpp` | Basic Reachability | Ensure objects on the stack (roots) are NOT collected. |
| `test_gc_unreachable.cpp` | Unreachable Objects | Ensure objects NOT on the stack ARE collected. |
| `test_transitive.cpp` | Transitive Refs | If Stack -> A -> B, ensure B is preserved even if not directly on stack. |
| `test_gc_cycles.cpp` | Cyclic References | Create a cycle (A->B, B->A) and drop stack ref. Ensure GC collects both. |
| `test_deep_object_graph.cpp` | Deep Graphs | Create a long linked list (1000 nodes) to test recursion depth limits. |
| `test_closure_capture.cpp` | Closures | Simulate a function closure (Func + Env) to ensure environment isn't freed. |
| `test_stress_allocation.cpp` | Stress Test | Allocate 300,000 objects into a 120,000 slot heap to force multiple GC cycles. |

## 💻 Technical Implementation Details

### The Object Struct

All heap objects are defined by the Object struct in `src/gc/gc.h`:

```cpp
struct Object {
    bool marked;       // The "Mark" bit
    Object* left;      // Reference 1 (e.g., CAR in Lisp)
    Object* right;     // Reference 2 (e.g., CDR in Lisp)
};
```

### The Allocation Strategy

The VM maintains a `free_list` pointer.

- **Allocation**: `new_pair` pops an object from the `free_list`.
- **Exhaustion**: If `free_list` is NULL, the GC is triggered (simulated in `test_stress_allocation.cpp`).
- **Collection**: The GC rebuilds the `free_list` by linking together all unmarked objects found during the sweep phase.

