#include "../src/vm/bvm.h"
#include "../src/gc/gc.h"
#include <stdio.h>
#include <iostream>
#include <chrono> // CHANGE: Include for timing

int main() {
    std::cout << "\n--- Test 1.6.7: Stress Allocation ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // CHANGE: Increase iterations > HEAP_SIZE (120,000) to force GC
    int iterations = 300000; 
    int success_count = 0;
    long long total_freed_objects = 0; // CHANGE: Track freed objects
    int gc_trigger_count = 0;          // CHANGE: Track how often GC ran
    
    printf("Attempting %d allocations (relying on GC)...\n", iterations);

    // CHANGE: Start Timer
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {
        Object* obj = new_pair(&vm, NULL, NULL);

        if (obj == NULL) {
            // Heap full, trigger GC
            int freed = gc(&vm); // CHANGE: Capture return value
            
            total_freed_objects += freed;
            gc_trigger_count++;

            // Try allocation again
            obj = new_pair(&vm, NULL, NULL);
            
            if (obj == NULL) {
                printf("[FAILED] Out of memory even after GC at iteration %d\n", i);
                return 1;
            }
        }
        success_count++;
    }

    // Final cleanup (optional, but good for accounting)
    total_freed_objects += gc(&vm);

    // CHANGE: Stop Timer
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    // --- REPORT METRICS ---
    printf("\n=== Performance Metrics ===\n");
    printf("Total Allocations:  %d\n", success_count);
    printf("Execution Time:     %.4f seconds\n", duration.count());
    printf("GC Triggered:       %d times\n", gc_trigger_count);
    printf("Total Objects Freed: %lld\n", total_freed_objects);
    printf("===========================\n");

    // Existing validation logic...
    int free_nodes = 0;
    Object* curr = vm.free_list;
    while(curr) {
        free_nodes++;
        curr = curr->right;
    }

    if (free_nodes == HEAP_SIZE) {
        printf("[PASSED] Heap empty after final GC.\n");
    } else {
        printf("[FAILED] Heap dirty. Free nodes: %d / %d\n", free_nodes, HEAP_SIZE);
    }

    return 0;
}