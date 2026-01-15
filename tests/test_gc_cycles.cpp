#include "../src/vm/bvm.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    printf("--- Running Test 1.6.4: Cyclic References ---\n");

    // 1. Initialize VM
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 2. Allocate objects
    // Obj* a = new_pair(NULL, NULL);
    printf("Allocating object 'a'...\n");
    Object* a = vm.new_pair(NULL, NULL);

    // Obj* b = new_pair(a, NULL);
    printf("Allocating object 'b' (points to a)...\n");
    Object* b = vm.new_pair(a, NULL);
    
    // 3. Create the Cycle: a->right = b
    printf("Creating cycle: a->right = b...\n");
    a->right = b;

    if (a == NULL || b == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // 4. Push 'a' to stack (Root Discovery)
    // Only 'a' is on the stack, but 'b' should be found via the cycle.
    printf("Pushing 'a' to stack...\n");
    vm.stack[vm.st_ptr] = (long long) a; 
    vm.st_ptr++;

    // 5. Run Garbage Collector
    printf("Triggering GC...\n");
    vm.gc();

    // 6. Verify: Both 'a' and 'b' should SURVIVE.
    // They should NOT be in the free_list.
    Object* curr = vm.free_list;
    bool found_garbage = false;
    
    while(curr) {
        if (curr == a) {
            printf("[FAILURE] Object 'a' was wrongly swept!\n");
            found_garbage = true;
        }
        if (curr == b) {
            printf("[FAILURE] Object 'b' was wrongly swept!\n");
            found_garbage = true;
        }
        curr = curr->right;
    }

    if(!found_garbage) {
        printf("[SUCCESS] Test Passed: Cycle (a <-> b) was handled correctly.\n");
    } else {
        printf("[FAILURE] One or more objects were swept.\n");
    }
    
    return 0;
}