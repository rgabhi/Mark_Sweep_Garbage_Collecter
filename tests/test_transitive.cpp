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
    printf("--- Running Test 1.6.3: Transitive Reachability ---\n");

    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 1. Create structure: Stack -> b -> a
    printf("Allocating object 'a'...\n");
    Object* a = new_pair(&vm, NULL, NULL);

    printf("Allocating object 'b' (pointing to 'a')...\n");
    Object* b = new_pair(&vm, a, NULL);

    // 2. Push ONLY 'b' to stack
    printf("Pushing 'b' to stack...\n");
    push(&vm, b);

    // 3. Run GC
    printf("Triggering GC...\n");
    gc(&vm);

    // 4. Verify both survived
    bool a_alive = !is_freed(&vm, a);
    bool b_alive = !is_freed(&vm, b);

    if (a_alive && b_alive) {
        printf("[PASSED] Both 'a' and 'b' survived.\n");
    } else {
        printf("[FAILED] Objects missing: a=%s, b=%s\n", 
               a_alive ? "alive" : "freed", 
               b_alive ? "alive" : "freed");
    }

    return 0;
}