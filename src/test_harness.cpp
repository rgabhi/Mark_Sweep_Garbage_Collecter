#include "vm/bvm.h"
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

    //verify
    // test 1.6.1 
    // NOTE:: :::: wrong test since a->marked would always be false :::::
    // if (a->marked == true) {
    //     printf("Test Passed: Object 'a' survived!\n");
    // } else {
    //     printf("Test Failed: Object 'a' was swept (marked is false).\n");
    // }
    // right test
    Object* curr = vm.free_list;
    bool found = false;
    while(curr){
        if (curr == a){
            found = true;
            break;
        }
        curr = curr->right;
    }
    if(found){
        printf("Test Failed: Object 'a' was swept (marked is false).\n");
    }
    else{
        printf("Test Passed: Object 'a' survived!\n");
    }

    
    return 0;
}
