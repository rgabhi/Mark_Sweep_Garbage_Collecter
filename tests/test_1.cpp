#include "../src/vm/bvm.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {

    // 1. Initialize VM
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code); 

    // 2. Allocate object 'a'
    printf("Allocating object 'a'...\n");
    Object* a = vm.new_pair(NULL, NULL);
    
    if (a == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    // printf("Pushing 'a' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) a; 
    // vm.st_ptr++;


    vm.gc();

    Object* b = vm.new_pair(NULL, NULL);
    if (b == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    // printf("Pushing 'b' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) b;
    // vm.st_ptr++;
    Object* c = vm.new_pair(NULL, NULL);
    if (c == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    // printf("Pushing 'c' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) c;
    // vm.st_ptr++;
    Object* f = vm.new_pair(NULL, NULL);
    if (f == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }

    Object* d = vm.new_pair(NULL, NULL);
    if (d == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    Object* e = vm.new_pair(NULL, NULL);
    if (e == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    // printf("Pushing 'f' to stack...\n");
    // vm.stack[vm.st_ptr] = (long long) f;
    // vm.st_ptr++;
    // Object* d = vm.new_pair(NULL, NULL);
    // Object* e = vm.new_pair(NULL, NULL);

    vm.gc();

    return 0;
}

