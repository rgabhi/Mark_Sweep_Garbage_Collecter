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

int main() {
    std::cout << "\n--- Test 1.6.5: Deep Object Graph ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // HEAP_SIZE is defined in bvm.h (assumed 1024 or larger)
    // We leave some buffer space.
    int list_size = HEAP_SIZE - 50; 
    if (list_size < 10) list_size = 50; // Safety check

    printf("Allocating linked list of %d objects...\n", list_size);

    Object* root = new_pair(&vm, NULL, NULL);
    Object* cur = root;

    // Create list: root -> node -> node ...
    for (int i = 0; i < list_size; i++) {
        Object* next = new_pair(&vm, NULL, NULL);
        if (!next) {
            printf("Heap exhaustion at index %d (expected if heap small)\n", i);
            break;
        }
        cur->right = next;
        cur = next;
    }

    // Root the start of the list
    push(&vm, root);

    printf("Running GC...\n");
    gc(&vm);

    // Verification
    if (is_freed(&vm, root)) {
        printf("[FAILED] Root object was collected.\n");
    } else if (is_freed(&vm, cur)) {
        printf("[FAILED] Tail object was collected (recursion issue).\n");
    } else {
        printf("[PASSED] Root and Tail objects survived.\n");
    }
    return 0;
}