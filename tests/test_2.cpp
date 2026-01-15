#include "../src/vm/bvm.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

    //dummy buffer
    unsigned char code[CODE_SIZE] = {0};
    VM vm(code);

    // allocate object in heap
    printf("Allocating object 'a'...\n");
    Object* a = vm.new_pair(NULL, NULL);

    if (a == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    
    //push a to stack
    // We cast the pointer to 'int' to store it in the stack array.
    // (Note: This assumes pointers fit in integers!)
    printf("Pushing 'a' to stack...\n");
    vm.stack[vm.st_ptr] = (long long) a;
    vm.st_ptr++;

    printf("Running GC....\n");
    vm.gc();

    Object *b=vm.new_pair(NULL,NULL);

     if (b == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    
    //push a to stack
    // We cast the pointer to 'int' to store it in the stack array.
    // (Note: This assumes pointers fit in integers!)
    printf("Pushing 'b' to stack...\n");
    vm.stack[vm.st_ptr] = (long long) b;
    vm.st_ptr++;

      Object *c=vm.new_pair(NULL,NULL);

     if (c == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    
    //push a to stack
    // We cast the pointer to 'int' to store it in the stack array.
    // (Note: This assumes pointers fit in integers!)
    printf("Pushing 'c' to stack...\n");
    vm.stack[vm.st_ptr] = (long long) c;
    vm.st_ptr++;

    Object *d=vm.new_pair(NULL,NULL);


     if (d == NULL) {
        printf("Error: Allocation failed!\n");
        return 1;
    }
    
    //push a to stack
    // We cast the pointer to 'int' to store it in the stack array.
    // (Note: This assumes pointers fit in integers!)
    printf("Pushing 'd' to stack...\n");
    vm.stack[vm.st_ptr] = (long long) b;
    vm.st_ptr++;








    //verify
    // test 1.6.1 
    // NOTE:: :::: wrong test since a->marked would always be false :::::
    // if (a->marked == true) {
    //     printf("Test Passed: Object 'a' survived!\n");
    // } else {
    //     printf("Test Failed: Object 'a' was swept (marked is false).\n");
    // }
    // right test
   vm.gc();

    
    return 0;
}