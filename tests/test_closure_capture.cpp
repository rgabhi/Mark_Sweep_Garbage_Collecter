#include "../src/vm/bvm.h"
#include "../src/gc/gc.h"
#include <stdio.h>
#include <iostream>

bool is_freed(VM* vm, Object* obj) {
    Object* curr = vm->free_list;
    while (curr) {
        if (curr == obj) return true;
        curr = curr->right;
    }
    return false;
}

// Helper wrappers specifically for this test context
Object* new_function(VM* vm) {
    return new_pair(vm, NULL, NULL);
}

Object* new_closure(VM* vm, Object* fn, Object* env) {
    return new_pair(vm, fn, env);
}

int main() {
    std::cout << "\n--- Test 1.6.6: Closure Capture ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // 1. Create Environment and Function
    Object* env = new_pair(&vm, NULL, NULL);
    Object* fn = new_function(&vm);

    // 2. Create Closure (captures fn and env)
    Object* cl = new_closure(&vm, fn, env);

    if (!env || !fn || !cl) {
        printf("Error: Allocation failed.\n");
        return 1;
    }

    // 3. Push ONLY the closure to the stack
    push(&vm, cl);

    printf("Running GC...\n");
    gc(&vm);

    // 4. Verification
    bool cl_alive = !is_freed(&vm, cl);
    bool fn_alive = !is_freed(&vm, fn);
    bool env_alive = !is_freed(&vm, env);

    if (cl_alive && fn_alive && env_alive) {
        printf("[PASSED] Closure, Function, and Environment survived.\n");
    } else {
        printf("[FAILED] Missing objects:\n");
        if (!cl_alive) printf(" - Closure\n");
        if (!fn_alive) printf(" - Function\n");
        if (!env_alive) printf(" - Environment\n");
    }

    return 0;
}