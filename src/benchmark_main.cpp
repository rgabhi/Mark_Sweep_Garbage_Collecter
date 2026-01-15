#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <map>
#include "vm/bvm.h"
#include "assembler/assembler.h"

// 1. CALCULATE PROGRAM SIZE
// This helper assembles the file in memory and returns the 'pc' (Program Counter),
// which represents the total bytes generated.
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
    return pc; // <--- THIS RETURNS THE PROGRAM SIZE IN BYTES
}

void run_benchmark(const std::string& name, const char* filepath) {
    uint8_t code_buffer[CODE_SIZE];
    memset(code_buffer, 0, CODE_SIZE);

    // Get Program Size
    int program_size = assemble_file(filepath, code_buffer);
    
    if (program_size < 0) {
        std::cout << std::left << std::setw(25) << name << " [File Not Found]" << std::endl;
        return;
    }

    int iterations = 100000;
    long long total_instructions = 0;
    
    // Start Timer
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<iterations; i++) {
        VM vm(code_buffer);
        vm.run();
        
        // 2. CAPTURE INSTRUCTION COUNT
        // We only need to grab this from one run (the first one) since it's deterministic
        if (i == 0) total_instructions = vm.getInstructionCnt();
    }

    // Stop Timer
    auto end = std::chrono::high_resolution_clock::now();

    // 3. CALCULATE AVERAGE TIME
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avg_time = total_duration.count() / (double)iterations;

    // Output results formatted as a table
    std::cout << std::left << std::setw(25) << name 
              << std::left << std::setw(15) << program_size        // Size
              << std::left << std::setw(20) << total_instructions  // Instructions
              << std::left << std::setw(15) << avg_time            // Time
              << std::endl;
}