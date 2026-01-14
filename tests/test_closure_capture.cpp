#include "../src/vm/bvm.h"
#include <stdio.h>
#include <iostream>


bool is_freed(VM& vm, Object* obj) {
    Object* curr = vm.free_list;
    while (curr) {
        if (curr == obj) return true;
        curr = curr->right;
    }
    return false;
}


Object* new_function(VM& vm) {
    return vm.new_pair(NULL, NULL);
}

Object* new_closure(VM& vm, Object* fn, Object* env) {
    return vm.new_pair(fn, env);
}


void test_closure_capture() {
    std::cout << "\n--- Test 1.6.6: Closure Capture ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // 1. Create Environment
    Object* env = vm.new_pair(NULL, NULL);
    
    // 2. Create Function (simulated)
    Object* fn = new_function(vm);

    // 3. Create Closure (captures fn and env)
    Object* cl = new_closure(vm, fn, env);

    if (!env || !fn || !cl) {
        printf("Error: Allocation failed during setup.\n");
        return;
    }

    // 4. Push ONLY the closure to the stack
    // env and fn are NOT on the stack, they are only reachable via cl
    vm.stack[vm.st_ptr++] = (long long)cl;

    printf("Running GC...\n");
    vm.gc();

    // Verification
    bool cl_alive = !is_freed(vm, cl);
    bool fn_alive = !is_freed(vm, fn);
    bool env_alive = !is_freed(vm, env);

    if (cl_alive && fn_alive && env_alive) {
        printf("[PASSED] Closure, Function, and Environment all survived.\n");
    } else {
        printf("[FAILED] Objects missing:\n");
        if (!cl_alive) printf(" - Closure collected\n");
        if (!fn_alive) printf(" - Function collected\n");
        if (!env_alive) printf(" - Environment collected\n");
    }
}


int main() {
    
    test_closure_capture();
    
    return 0;
}