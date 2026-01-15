#include "../src/vm/bvm.h"
#include "../src/gc/gc.h"
#include <stdio.h>
#include <chrono>

bool is_freed(VM* vm, Object* obj) {
    Object* curr = vm->free_list;
    while (curr) {
        if (curr == obj) return true;
        curr = curr->right;
    }
    return false;
}

int main() {
    printf("\n--- Test 1.6.6: Closure Capture ---\n");
    auto start = std::chrono::high_resolution_clock::now();

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    Object* env = new_pair(&vm, NULL, NULL);
    Object* fn = new_function(&vm);
    Object* cl = new_closure(&vm, fn, env);
    push(&vm, cl);

    int freed = gc(&vm);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    printf("\n=== Performance Metrics ===\n");
    printf("Execution Time:     %.6f seconds\n", duration.count());
    printf("Total Objects Freed: %d\n", freed);
    printf("===========================\n");

    if (!is_freed(&vm, cl) && !is_freed(&vm, fn) && !is_freed(&vm, env)) {
        printf("[PASSED] All closure components survived.\n");
    } else {
        printf("[FAILED] Components missing.\n");
    }
    return 0;
}