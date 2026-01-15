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
    printf("--- Running Test 1.6.1: Basic Reachability ---\n");

    // 1. Initialize VM
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 2. Allocate object 'a'
    printf("Allocating object 'a'...\n");
    Object* a = new_pair(&vm, NULL, NULL);
    
    if (a == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // 3. Push 'a' to stack (Root it)
    printf("Pushing 'a' to stack...\n");
    push(&vm, a);

    // 4. Run GC
    printf("Triggering GC...\n");
    gc(&vm);

    // 5. Verify Survival
    if (!is_freed(&vm, a)) {
        printf("[PASSED] Object 'a' survived.\n");
    } else {
        printf("[FAILED] Object 'a' was swept!\n");
    }

    return 0;
}