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
    printf("--- Running Test 1.6.3: Transitive Reachability ---\n");
    auto start = std::chrono::high_resolution_clock::now();

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    Object* a = new_pair(&vm, NULL, NULL);
    Object* b = new_pair(&vm, a, NULL);
    push(&vm, b); // Only root b

    int freed = gc(&vm);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    printf("\n=== Performance Metrics ===\n");
    printf("Execution Time:     %.6f seconds\n", duration.count());
    printf("Total Objects Freed: %d\n", freed);
    printf("===========================\n");

    if (!is_freed(&vm, a) && !is_freed(&vm, b)) {
        printf("[PASSED] Both 'a' and 'b' survived.\n");
    } else {
        printf("[FAILED] Objects missing.\n");
    }
    return 0;
}