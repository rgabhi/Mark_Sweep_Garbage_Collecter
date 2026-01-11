# Bytecode Virtual Machine (BVM)

This project implements a complete virtual machine capable of executing bytecode programs with support for arithmetic operations, control flow, function calls, and memory management.

## 🚀 Features

- **Stack-Based Architecture**: Efficient stack-based execution model
- **Custom Assembly Language**: Human-readable assembly syntax with label support
- **Two-Pass Assembler**: Converts assembly code to bytecode with automatic address resolution
- **Rich Instruction Set**: 17 opcodes covering arithmetic, logic, control flow, and memory operations
- **Function Call Support**: support for nested function calls with separate return stack
- **Memory Management**: 256 integer memory slots for variable storage
- **Benchmark Suite**: Performance testing framework with 10 test programs
- **Comprehensive Error Handling**: Stack overflow/underflow detection and division by zero checks

## 📋 Instruction Set

### Stack Operations
- `PUSH <value>` - Push integer value onto stack
- `POP` - Remove top value from stack
- `DUP` - Duplicate top stack value

### Arithmetic Operations
- `ADD` - Add top two values (a + b)
- `SUB` - Subtract (a - b)
- `MUL` - Multiply (a × b)
- `DIV` - Integer division (a ÷ b)
- `CMP` - Compare (pushes 1 if a < b, else 0)

### Control Flow
- `JMP <addr>` - Unconditional jump to address
- `JZ <addr>` - Jump if top value is zero
- `JNZ <addr>` - Jump if top value is non-zero

### Memory Operations
- `LOAD <index>` - Load value from memory[index] to stack
- `STORE <index>` - Store top value to memory[index]

### Function Calls
- `CALL <addr>` - Call function at address (supports labels)
- `RET` - Return from function

### Program Control
- `HALT` - Stop execution

## 🏗️ Architecture

```
┌─────────────────────────────────────────┐
│         Bytecode Virtual Machine        │
├─────────────────────────────────────────┤
│  Instruction Pointer (Program Counter)  │
│  Main Stack (1024 integers)             │
│  Return Stack (256 addresses)           │
│  Memory Array (256 integers)            │
└─────────────────────────────────────────┘
           ↑
           │ bytecode
           │
┌─────────────────────────────────────────┐
│               Assembler                 │
├─────────────────────────────────────────┤
│  Pass 1: Build Symbol Table (Labels)    │
│  Pass 2: Generate Bytecode              │
└─────────────────────────────────────────┘
           ↑
           │ .asm files
           │
┌─────────────────────────────────────────┐
│        Assembly Source Code             │
└─────────────────────────────────────────┘
```

## 🛠️ Building the Project

### Prerequisites
- C++ compiler with C++11 support (g++ recommended)
- Make utility

### Compilation

```bash
# Compile all components (VM, Assembler, Benchmark)
make all

# Compile individual components
make vm          # Compile only the VM
make assembler   # Compile only the Assembler
make benchmark   # Compile only the Benchmark tool

# Clean build artifacts
make clean
```

## 💻 Usage

### Running Assembly Programs

```bash
# Assemble an .asm file to bytecode
./assembler input.asm output.bin

# Execute the bytecode
./vm output.bin
```

### Example: Factorial Program

**factorial.asm**
```asm
; Calculate 5! = 120
PUSH 5
CALL factorial
HALT

factorial:
    DUP
    PUSH 2
    CMP
    JZ base_case
    ; Recursive case
    DUP
    PUSH 1
    SUB
    CALL factorial
    MUL
    RET
    
base_case:
    POP
    PUSH 1
    RET
```

**Run it:**
```bash
./assembler factorial.asm factorial.bin
./vm factorial.bin
# Output: Final stack top: 120
```

### Running All Tests

```bash
make run
```

This will:
1. Compile all components
2. Run all test cases in `tests/` folder


## 📊 Test Suite

The project includes 10 comprehensive test programs:

| Test | Description | Expected Output |
|------|-------------|----------------|
| `test1_arithmetic.asm` | Basic arithmetic operations | 42 |
| `test2_circle_area.asm` | Circle area calculation (πr²) | 314 |
| `test3_loop_sum.asm` | Sum of 1 to 10 | 55 |
| `test4_factorial.asm` | Factorial (5!) with recursion | 120 |
| `test5_fibonacci.asm` | Fibonacci number calculation | Varies |
| `test6_nested_calls.asm` | Nested function calls | 49 |
| `test7_memory_ops.asm` | Memory load/store operations | Varies |
| `test8_conditional.asm` | Conditional branching | Varies |
| `test9_stack_ops.asm` | Stack manipulation (DUP, POP) | Varies |
| `test10_complex.asm` | Complex multi-operation program | Varies |

## 🎯 Benchmark Results

Run the benchmark suite to measure performance:

```bash
./benchmark
```

**Sample Output:**
```
=========================================================================
Test Case                Size (Bytes)   Instructions Exec   Time (us)      
=========================================================================
Arithmetic               24             8                   0.08509        
Circle Area              14             6                   0.08916        
Loop Sum                 79             135                 1.12305        
Factorial (Func)         73             63                  0.50169        
Fibonacci                94             120                 0.91607        
Nested Calls             26             10                  0.08051        
Memory Ops               64             16                  0.14606        
Conditional              52             11                  0.08892        
Stack Ops                32             12                  0.10955        
Complex Calc             82             86                  0.69301 
...
=========================================================================
```

The benchmark runs each test 100,000 times and reports:
- **Program Size**: Bytecode size in bytes
- **Instructions Executed**: Total VM instructions per run
- **Average Time**: Execution time in microseconds

## 📁 Project Structure

```
Bytecode_Virtual_Machine/
├── src/
│   ├── vm/
│   │   ├── bvm.h              # VM class definition
│   │   ├── bvm.cpp            # VM implementation
│   │   └── bvm_main.cpp       # VM entry point
│   ├── assembler/
│   │   ├── assembler.h        # Assembler function declarations
│   │   ├── assembler.cpp      # Two-pass assembler implementation
│   │   └── assembly_main.cpp  # Assembler entry point
│   ├── benchmark_main.cpp     # Benchmark suite
│   └── commons.h              # Shared opcode definitions
├── tests/
│   ├── test1_arithmetic.asm
│   ├── test2_circle_area.asm
│   ├── test3_loop_sum.asm
│   ├── test4_factorial.asm
│   ├── test5_fibonacci.asm
│   ├── test6_nested_calls.asm
│   ├── test7_memory_ops.asm
│   ├── test8_conditional.asm
│   ├── test9_stack_ops.asm
│   └── test10_complex.asm
├── Makefile
└── README.md
```

## 🔧 Technical Details

### Memory Layout
- **Code Buffer**: 1024 bytes for bytecode
- **Main Stack**: 1024 integer slots
- **Return Stack**: 256 address slots
- **Memory Array**: 256 integer slots
- **Line Buffer**: 128 characters for assembly parsing

### Bytecode Format
- **Instructions without arguments**: 1 byte (opcode only)
- **Instructions with arguments**: 5 bytes (1 byte opcode + 4 byte integer)

### Error Handling
- Stack overflow detection (main and return stacks)
- Stack underflow detection
- Division by zero protection
- Undefined label detection during assembly
- File I/O error handling

## 🎓 Learning Objectives

This project demonstrates:
- Virtual machine design and implementation
- Stack-based computation models
- Two-pass assembly with symbol resolution
- Bytecode generation and execution
- Function call conventions and return stack management
- Performance benchmarking and profiling
- Low-level systems programming in C++


## 📝 Assembly Language Syntax

```asm
; Comments start with semicolon

; Labels define jump targets
label_name:
    INSTRUCTION arg

; Labels can be on same line as instruction
another_label: INSTRUCTION

; Numeric arguments
PUSH 42          ; Decimal integers
JMP 100          ; Absolute addresses

; Label arguments (resolved by assembler)
CALL function_name
JZ loop_start
```

## 👨‍💻 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation
- Add more test cases

## 🙏 Acknowledgments

Built as an educational project to explore:
- Virtual machine architecture
- Compiler/assembler design
- Low-level programming concepts
- Performance optimization techniques

---

**Made with ❤️ for learning systems programming**