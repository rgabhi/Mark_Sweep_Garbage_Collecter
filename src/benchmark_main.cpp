#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <map>
#include "vm/bvm.h"
#include "assembler/assembler.h"

// Helper to load and assemble a file in memory
int assemble_file(const char* filename, uint8_t* code_buffer) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        return -1;
    }

    int pc = 0;
    char line[LINE_SIZE];
    std::map<std::string, int> labels;

    // --- PASS 1: Parse Labels ---
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == '\n' || line[0] == ';') continue;
        pc = parse_labels(line, pc, labels);
    }

    // Reset file pointer for Pass 2
    fseek(in, 0, SEEK_SET);
    pc = 0;

    // --- PASS 2: Generate Code ---
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == '\n' || line[0] == ';') continue;
        // Pass labels map to the updated function
        pc = assemble_line(line, code_buffer, pc, labels);
    }
    
    fclose(in);
    return pc; // Returns total bytes (Program Size)
}

void run_benchmark(const std::string& name, const char* filepath) {
    uint8_t code_buffer[CODE_SIZE];
    memset(code_buffer, 0, CODE_SIZE);

    int program_size = assemble_file(filepath, code_buffer);
    
    if (program_size < 0) {
        std::cout << std::left << std::setw(25) << name << " [File Not Found]" << std::endl;
        return;
    }

    int iterations = 100000;
    long long total_instructions = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<iterations; i++) {
        VM vm(code_buffer);
        vm.run();
        if (i == 0) total_instructions = vm.getInstructionCnt();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avg_time = total_duration.count() / (double)iterations;

    std::cout << std::left << std::setw(25) << name 
              << std::left << std::setw(15) << program_size 
              << std::left << std::setw(20) << total_instructions 
              << std::left << std::setw(15) << avg_time 
              << std::endl;
}

int main() {
    struct TestCase {
        std::string name;
        const char* path;
    };

    std::vector<TestCase> tests = {
        {"Arithmetic", "tests/test1_arithmetic.asm"},
        {"Circle Area", "tests/test2_circle_area.asm"},
        {"Loop Sum", "tests/test3_loop_sum.asm"},
        {"Factorial (Func)", "tests/test4_factorial.asm"},
        {"Fibonacci", "tests/test5_fibonacci.asm"},
        {"Nested Calls", "tests/test6_nested_calls.asm"},
        {"Memory Ops", "tests/test7_memory_ops.asm"},
        {"Conditional", "tests/test8_conditional.asm"},
        {"Stack Ops", "tests/test9_stack_ops.asm"},
        {"Complex Calc", "tests/test10_complex.asm"}
    };

    std::cout << "=========================================================================\n";
    std::cout << std::left << std::setw(25) << "Test Case" 
              << std::left << std::setw(15) << "Size (Bytes)" 
              << std::left << std::setw(20) << "Instructions Exec" 
              << std::left << std::setw(15) << "Time (us)" 
              << std::endl;
    std::cout << "=========================================================================\n";

    for (const auto& test : tests) {
        run_benchmark(test.name, test.path);
    }
    std::cout << "=========================================================================\n";

    return 0;
}