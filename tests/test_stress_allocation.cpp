#include "../src/vm/bvm.h"
#include <stdio.h>
#include <iostream>


void test_stress_allocation() {
    std::cout << "\n--- Test 1.6.7: Stress Allocation ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    int iterations = 100000;
    int success_count = 0;
    
    printf("Attempting %d allocations (with intermediate GCs)...\n", iterations);

    for (int i = 0; i < iterations; i++) {
        // Allocate object (without rooting it)
        Object* obj = vm.new_pair(NULL, NULL);

        // If heap is full, the current BVM returns NULL. 
        // We must manually trigger GC to reclaim the unrooted garbage from previous iterations.
        if (obj == NULL) {
            vm.gc();
            
            // Try allocation again
            obj = vm.new_pair(NULL, NULL);
            
            if (obj == NULL) {
                printf("[FAILED] Out of memory even after GC at iteration %d\n", i);
                return;
            }
        }
        success_count++;
    }

    // Final cleanup
    vm.gc();

    // Check if Heap is empty (since nothing was ever rooted)
    int free_nodes = 0;
    Object* curr = vm.free_list;
    while(curr) {
        free_nodes++;
        curr = curr->right;
    }

    if (free_nodes == HEAP_SIZE) {
        printf("[PASSED] Completed %d allocations. Heap is completely empty after final GC.\n", success_count);
    } else {
        printf("[FAILED] Heap dirty. Free nodes: %d / %d\n", free_nodes, HEAP_SIZE);
    }
}

int main() {
  
    test_stress_allocation();
    return 0;
}