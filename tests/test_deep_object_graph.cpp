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
    printf("--- Running Test 1.6.5: Deep Object Graph ---\n");
    auto start = std::chrono::high_resolution_clock::now();

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    int list_size = 1000; // Large list
    Object* root = new_pair(&vm, NULL, NULL);
    Object* cur = root;

    for (int i = 0; i < list_size; i++) {
        Object* next = new_pair(&vm, NULL, NULL);
        if(!next) break; 
        cur->right = next;
        cur = next;
    }
    push(&vm, root);

    int freed = gc(&vm);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    printf("\n=== Performance Metrics ===\n");
    printf("Execution Time:     %.6f seconds\n", duration.count());
    printf("Total Objects Freed: %d\n", freed);
    printf("===========================\n");

    if (!is_freed(&vm, root) && !is_freed(&vm, cur)) {
        printf("[PASSED] Root and Tail objects survived.\n");
    } else {
        printf("[FAILED] Objects collected.\n");
    }
    return 0;
}