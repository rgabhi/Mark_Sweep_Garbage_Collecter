#include "../src/vm/bvm.h"
#include "../src/gc/gc.h"
#include <stdio.h>
#include <iostream>

int main() {
    std::cout << "\n--- Test 1.6.7: Stress Allocation ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    int iterations = 100000;
    int success_count = 0;
    
    printf("Attempting %d allocations (relying on GC)...\n", iterations);

    for (int i = 0; i < iterations; i++) {
        // Allocate object (without rooting it, making it garbage immediately)
        Object* obj = new_pair(&vm, NULL, NULL);

        // If heap is full, manually trigger GC to reclaim the unrooted garbage
        if (obj == NULL) {
            gc(&vm);
            
            // Try allocation again
            obj = new_pair(&vm, NULL, NULL);
            
            if (obj == NULL) {
                printf("[FAILED] Out of memory even after GC at iteration %d\n", i);
                return 1;
            }
        }
        success_count++;
    }

    // Final cleanup
    gc(&vm);

    // Check if Heap is effectively empty (everything was unrooted garbage)
    int free_nodes = 0;
    Object* curr = vm.free_list;
    while(curr) {
        free_nodes++;
        curr = curr->right;
    }

    if (free_nodes == HEAP_SIZE) {
        printf("[PASSED] %d allocations succeeded. Heap empty after final GC.\n", success_count);
    } else {
        printf("[FAILED] Heap dirty. Free nodes: %d / %d\n", free_nodes, HEAP_SIZE);
    }

    return 0;
}