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
    printf("--- Running Test 1.6.4: Cyclic References ---\n");

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 1. Allocate objects
    Object* a = new_pair(&vm, NULL, NULL);
    Object* b = new_pair(&vm, a, NULL); // b -> a
    
    if (!a || !b) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // 2. Create Cycle: a -> b
    printf("Creating cycle a <-> b...\n");
    a->right = b;

    // 3. Push 'a' to stack
    printf("Pushing 'a' to stack...\n");
    push(&vm, a);

    // 4. Run GC
    gc(&vm);

    // 5. Verify Survival
    if (!is_freed(&vm, a) && !is_freed(&vm, b)) {
        printf("[PASSED] Cycle handled correctly (both survived).\n");
    } else {
        printf("[FAILED] Cycle objects were collected.\n");
    }
    
    return 0;
}