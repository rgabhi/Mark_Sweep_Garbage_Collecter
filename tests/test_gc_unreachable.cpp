#include "../src/vm/bvm.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    printf("--- Running Test 1.6.2: Unreachable Object Collection ---\n");

    // 1. Initialize VM
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 2. Allocate object 'a'
    printf("Allocating object 'a'...\n");
    Object* a = vm.new_pair(NULL, NULL);
    
    if (a == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // 3. SKIP PUSH (The object is now unreachable!)
    printf("Skipping push... 'a' is now floating garbage.\n");

    // 4. Run Garbage Collector
    printf("Triggering GC...\n");
    vm.gc();

    // 5. Verify: We WANT to find 'a' in the free_list
    Object* curr = vm.free_list;
    bool found = false;
    
    while(curr) {
        if (curr == a) {
            found = true;
            break;
        }
        curr = curr->right;
    }

    if(found) {
        printf("[SUCCESS] Test Passed: Object 'a' was correctly swept/freed.\n");
    } else {
        printf("[FAILURE] Test Failed: Object 'a' is missing from free list (memory leak).\n");
    }
    
    return 0;
}
    // Object* b = vm.new_pair(NULL, NULL);
    // if (b == NULL) {
    //     printf("Error: Allocation failed!\n");
    //     return 1;
    // }
    // printf("Pushing 'b' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) b;
    // vm.st_ptr++;
    // Object* c = vm.new_pair(NULL, NULL);
    // if (c == NULL) {
    //     printf("Error: Allocation failed!\n");
    //     return 1;
    // }
    // printf("Pushing 'a' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) c;
    // vm.st_ptr++;
    // Object* f = vm.new_pair(NULL, NULL);
    // if (f == NULL) {
    //     printf("Error: Allocation failed!\n");
    //     return 1;
    // }
    // Object* d = vm.new_pair(NULL, NULL);
    // Object* e = vm.new_pair(NULL, NULL);

    // vm.gc();

    // return 0;
