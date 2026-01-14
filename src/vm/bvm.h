#ifndef BVM_H   
#define BVM_H

#define STACK_SIZE 1024
#define MEM_SIZE   256 // memory
#define CODE_SIZE  1024 
#define RET_STACK_SIZE 256

#define LINE_SIZE 128
#define HEAP_SIZE 1024 // define heap


// #include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <cstring>

struct Object{
    bool marked; // for garbage collection
    Object* left;
    Object* right;
};


class VM {
    public:
        // int pc;
        // uint8_t code[CODE_SIZE];
        //program
        unsigned char* program; // ptr to first byte code
        unsigned char* inst_ptr; // instrucion ptr (PC): points to curr instructn
        long long instruction_cnt;
        bool running;

        //stack
        long long stack[STACK_SIZE];
        int st_ptr; // stack ptr

        // memory
        int memory[MEM_SIZE];


        int ret_stack[RET_STACK_SIZE];
        int rst_ptr;

        //heap
        Object heap[HEAP_SIZE];
        Object* free_list; // to track empty slots

        VM(unsigned char* bytecode);
        void run();
        bool check_stack(int count);
        long long getInstructionCnt();
        //heap methods
        Object* new_pair(Object* l, Object* r);
        void mark_object(Object *obj);
        void gc();
};



#endif