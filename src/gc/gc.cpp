#include "gc.h"
#include "../vm/bvm.h" // Include full VM definition to access stack/heap
#include <stdio.h>



// --- Helper func ---

// mark_obj
void mark_object(Object* obj) {
    if (obj == NULL || obj->marked) {
        return;
    }
    
    obj->marked = true;
    
    // recursively mark
    mark_object(obj->left);
    mark_object(obj->right);
}

// --- wrappers ---

// HEAP
// new_pair
Object* new_pair(VM* vm, Object* l, Object* r) {
    if (vm->free_list == NULL) {
        // Simple Fail behavior: print error and return NULL
        // In a real VM, you might trigger GC() here automatically.
        printf("Heap Overflow\n");
        return NULL;
    }

    Object* curr = vm->free_list;
    vm->free_list = vm->free_list->right;

    curr->left = l;
    curr->right = r;
    curr->marked = false;

    return curr;
}

void push(VM* vm, Object* obj) {
    if (vm->st_ptr >= STACK_SIZE) {
        printf("Stack Overflow\n");
        return;
    }
    // store ptr as integer on the stack
    vm->stack[vm->st_ptr] = (long long)obj;
    vm->st_ptr++;
}

Object* pop(VM* vm) {
    if (vm->st_ptr <= 0) {
        printf("Stack Underflow\n");
        return NULL;
    }
    vm->st_ptr--;
    return (Object*)vm->stack[vm->st_ptr];
}

void gc(VM* vm) {
    // mark
    // iterate through the VM stack to find roots
    for (int i = 0; i < vm->st_ptr; i++) {
        Object* obj = (Object*)vm->stack[i]; // cast int to ptr
        
        if (obj >= vm->heap && obj < vm->heap + HEAP_SIZE) {
            // valid ptr in our heap range
            mark_object(obj);
        }
    }

    // sweep
    vm->free_list = NULL;
    Object* curr_free = NULL; // tracks end of new free_list

    for (int i = 0; i < HEAP_SIZE; i++) {
        if (vm->heap[i].marked) {
            // obj is alive, unmark it for next cycle
            vm->heap[i].marked = false;
        } else {
            // obj is dead, add to free list
            if (vm->free_list == NULL) {
                vm->free_list = &vm->heap[i];
                curr_free = vm->free_list;
            } else {
                curr_free->right = &vm->heap[i];
                curr_free = curr_free->right;
            }
        }
    }

    // terminate the free list
    if (curr_free) {
        curr_free->right = NULL;
    }
}