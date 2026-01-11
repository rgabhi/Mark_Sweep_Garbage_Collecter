#ifndef BVM_H   
#define BVM_H

#define STACK_SIZE 1024
#define MEM_SIZE   256 // memory
#define CODE_SIZE  1024 
#define RET_STACK_SIZE 256

#define LINE_SIZE 128


// #include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <cstring>


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
        int stack[STACK_SIZE];
        int st_ptr; // stack ptr

        // memory
        int memory[MEM_SIZE];


        int ret_stack[RET_STACK_SIZE];
        int rst_ptr;

        VM(unsigned char* bytecode);
        void run();
        bool check_stack(int count);
        long long getInstructionCnt();
        
};



#endif