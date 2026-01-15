#include "../src/vm/bvm.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    printf("--- Running Test 1.6.5: Deep Object Graph (Stress Test) ---\n");

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 1. Create Root
    printf("Allocating root...\n");
    Object* root = vm.new_pair(NULL, NULL);
    Object* cur = root;

    // 2. Build a massive chain (10,000 deep)
    // Note: If HEAP_SIZE is only 1024, we need to handle that!
    // The default HEAP_SIZE in bvm.h is 1024. 
    // We can't allocate 10,000 objects if the heap only holds 1024.
    // Let's test the limit of the CURRENT heap (fill it up completely).
    
    int limit = HEAP_SIZE - 5; // Leave a little breathing room
    printf("Building chain of %d objects...\n", limit);

    for (int i = 0; i < limit; i++) {
        Object* next = vm.new_pair(NULL, NULL);
        if (next == NULL) {
            printf("Heap full at %d objects!\n", i);
            break;
        }
        cur->right = next;
        cur = next;
    }

    // 3. Push ROOT to stack
    printf("Pushing root to stack...\n");
    vm.stack[vm.st_ptr] = (long long) root;
    vm.st_ptr++;

    // 4. Run GC
    // This will trigger mark_object recursively down the entire chain.
    printf("Triggering GC (Recursive Step)...\n");
    vm.gc();

    // 5. Verify Survival
    // Don't check cur->marked directly, because GC resets it to false!
    // Instead, check if 'cur' ended up in the free_list (trash).
    
    Object* check = vm.free_list;
    bool swept = false;
    while (check) {
        if (check == cur) {
            swept = true;
            break;
        }
        check = check->right;
    }

    if (!swept) {
        printf("[SUCCESS] Test Passed: Deep graph survived without crashing.\n");
    } else {
        printf("[FAILURE] The end of the chain was swept! (Found in free_list)\n");
    }

    return 0;
}