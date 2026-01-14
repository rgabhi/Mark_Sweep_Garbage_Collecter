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



void test_deep_object_graph() {
    std::cout << "\n--- Test 1.6.5: Deep Object Graph ---\n";
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // NOTE: HEAP_SIZE is defined as 1024 in bvm.h. 
    // Attempting 10,000 allocations would crash. We scale this to fit the VM size.
    int list_size = HEAP_SIZE - 50; 
    printf("Allocating linked list of %d objects...\n", list_size);

    Object* root = vm.new_pair(NULL, NULL);
    Object* cur = root;

    // Create a long linked list: root -> node -> node ...
    for (int i = 0; i < list_size; i++) {
        Object* next = vm.new_pair(NULL, NULL);
        if (!next) {
            printf("Error: Heap exhaustion at index %d\n", i);
            break;
        }
        cur->right = next;
        cur = next;
    }

    // Root the start of the list
    vm.stack[vm.st_ptr++] = (long long)root;

    printf("Running GC...\n");
    vm.gc();

    // Verification
    // If the recursive marker works, the entire list (reachable from root) must survive.
    if (is_freed(vm, root)) {
        printf("[FAILED] Root object was collected.\n");
    } else if (is_freed(vm, cur)) {
        printf("[FAILED] Tail object was collected (recursion didn't go deep enough).\n");
    } else {
        printf("[PASSED] Root and Tail objects survived.\n");
    }
}

int main() {
    test_deep_object_graph();
    
    return 0;
}