#include "../src/vm/bvm.h"
#include "../src/gc/gc.h"
#include <stdio.h>

bool is_freed(VM* vm, Object* obj) {
    Object* curr = vm->free_list;
    while (curr) {
        if (curr == obj) return true;
        curr = curr->right;
    }
    return false;
}

int main() {
    printf("--- Running Test 1.6.2: Unreachable Object Collection ---\n");

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 1. Allocate object 'a'
    printf("Allocating object 'a'...\n");
    Object* a = new_pair(&vm, NULL, NULL);
    
    if (a == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // 2. DO NOT Push 'a' to stack (It is unreachable)
    printf("Object 'a' created but NOT pushed to stack.\n");

    // 3. Run GC
    printf("Triggering GC...\n");
    gc(&vm);

    // 4. Verify Collection
    if (is_freed(&vm, a)) {
        printf("[PASSED] Object 'a' was correctly freed.\n");
    } else {
        printf("[FAILED] Object 'a' was NOT freed (Memory Leak).\n");
    }

    return 0;
}