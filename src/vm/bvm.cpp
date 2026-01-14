#include "bvm.h"
#include "../commons.h"


long long VM::getInstructionCnt(){return instruction_cnt;}

VM::VM(unsigned char* bytecode){
            this->program = bytecode;
            this->inst_ptr = bytecode;
            this->st_ptr = 0;
            this->rst_ptr = 0;
            this->instruction_cnt=0;
            // init memory to zero
            memset(this->memory, 0, sizeof(this->memory));
            // init heap
            for(int i = 0; i < HEAP_SIZE - 1; i++){
                this->heap[i].right = &this->heap[i + 1];
            }
            this->heap[HEAP_SIZE -1].right = NULL;
            //free list
            this->free_list = &this->heap[0];
}

 // HEAP
// new_pair
Object* VM::new_pair(Object* l, Object* r){
    if(free_list == NULL){
        printf("Heap Overflow");
        return NULL;
    }
    Object* curr = free_list;
    free_list = free_list->right;
    curr->left = l;
    curr->right = r;
    curr->marked = false;

    return curr;
}

// mark_obj
void VM::mark_object(Object* obj){
    if(obj == NULL){
        return;
    }
    if(obj->marked){
        return;
    }
    obj->marked=true;
    mark_object(obj->left);
    mark_object(obj->right);
}

// helper to check st size
bool VM::check_stack(int count) {
    if (this->st_ptr < count) {
        printf("Error: Stack Underflow\n");
        this->running = false;
        return false;
    }
    return true;
}


void VM::gc(){
    for(int i = 0; i < this->st_ptr; i++){
        Object* obj = (Object*) stack[i]; // cast int to ptr
        if(obj >= this->heap && obj < this->heap + HEAP_SIZE){
            // valid ptr in our heap range
            mark_object(obj);
        }
    }
    free_list = NULL;

for (int i = 0; i < HEAP_SIZE; i++) {
    Object* obj = &heap[i];
    if (!obj->marked) {
        obj->right = free_list;
        free_list = obj;
    } else {
        obj->marked = false; // reset for next GC
    }
}

}
 
void VM::run(){
    this->running = true;
    while(running){
        this->instruction_cnt++;// increment per inst cycle
        // 1. FETCH : get curr instruction
        unsigned char opcode = *this->inst_ptr;
     
        // 2. DECODE: decide what to do
        switch(opcode){
            case HALT: // HALT
            {
                running = false;
                break;
            }
            case PUSH: //PUSH
            {    // get addr of the byte after opcode
                // (int *):  cast it: treat addr as ptr to int
                // * : dereference: read int val
                if(st_ptr >= STACK_SIZE){
                    printf("Stack Overflow\n");
                    running = false;
                    break;
                }
                int val = *(int *)(this->inst_ptr + 1);
                // stack logic
                // add val to stack
                // update stack ptr
                this->stack[this->st_ptr] = val;
                this->st_ptr++; 

                // move to next inst by jumping 4 byte int
                this->inst_ptr += 4;
                break;
            }
            case POP: { // POP
                if(check_stack(1)){
                    this->st_ptr--;
                }
                break;
            }
            case ADD: // ADD
            {
                if(!check_stack(2)){
                    break;
                }
                this->st_ptr--;
                int b = this->stack[this->st_ptr];
                this->st_ptr--;
                int a = this->stack[this->st_ptr];
                b = a + b;
                this->stack[this->st_ptr] = b;
                this->st_ptr++;
                break;
            }
            case SUB: // SUBTRACT
            {
                if(!check_stack(2)){
                    break;
                }
                this->st_ptr--;
                int b = this->stack[this->st_ptr];
                this->st_ptr--;
                int a = this->stack[this->st_ptr];
                int res = a - b;
                this->stack[this->st_ptr] = res;
                this->st_ptr++;
                break;
            }
            case MUL: // MUL
            {
                if(!check_stack(2)){
                    break;
                }
                this->st_ptr--;
                int b = this->stack[this->st_ptr];
                this->st_ptr--;
                int a = this->stack[this->st_ptr];
                int res = a*b;
                this->stack[this->st_ptr] = res;
                this->st_ptr++;
                break;
            }
            case DIV: // DIV
            {
                if(!check_stack(2)){
                    break;
                }
                this->st_ptr--;
                int b = this->stack[this->st_ptr];
                if (b==0){
                    printf("Error: Div by Zero\n");
                    running = false;
                    break;
                }
                this->st_ptr--;
                int a = this->stack[this->st_ptr];
                int res = (int)a/b;
                this->stack[this->st_ptr] = res;
                this->st_ptr++;
                break;
            }
            case CMP: { // CMP (eqal check)
                if(!check_stack(2)){
                    break;
                }
                this->st_ptr--;
                int b = this->stack[this->st_ptr];
                this->st_ptr--;
                int a = this->stack[this->st_ptr];
                
                
               int res = (a < b) ? 1 : 0;
                
                this->stack[this->st_ptr] = res;
                this->st_ptr++;
                break;
            }

            case STORE: //  STORE IDX
            {
                // pop val from stack
                this->st_ptr--;
                int val =this->stack[this->st_ptr];
                // get idx 
                int idx = *(int *) (this->inst_ptr + 1);
                this->memory[idx] = val;
                // move inst_ptr
                this->inst_ptr += 4;
                break;
            }
            case LOAD: // LOAD IDX
            {
                if (st_ptr >= STACK_SIZE) {
                    printf("Stack Overflow\n");
                    running = false;
                    break;
                }
                // get idx
                int idx = *(int *)(this->inst_ptr + 1);
                
                // read val from memory
                int val = this->memory[idx];
                // push to stack
                this->stack[this->st_ptr] = val;
                this->st_ptr++;

                // move inst_ptr
                this->inst_ptr += 4;
                break;
            }
            
            case JMP: // JUMP addr
            {
                // get idx
                int target = *(int *)(this->inst_ptr + 1);

                //update ptr
                this->inst_ptr = this->program + target;
                //jump to next instr
                continue;
            }
            case JZ: // JZ addr
            {
                if(!check_stack(1)) break;
                // pop val
                this->st_ptr--;
                int val = this->stack[this->st_ptr];

                // target address
                int target = *(int *)(this->inst_ptr + 1);

                if(val == 0){
                    this->inst_ptr = this->program + target;
                    continue;
                }
                else{
                    //
                    this->inst_ptr += 4;
                    break;
                }
            }
            case JNZ: // JNZ addr
            {
                if (!check_stack(1)) break;
                // pop val
                this->st_ptr--;
                int val = this->stack[this->st_ptr];

                // target address
                int target = *(int *)(this->inst_ptr + 1);

                if(val != 0){
                    this->inst_ptr = this->program + target;
                    continue;
                }
                else{
                    //
                    this->inst_ptr += 4;
                    break;
                }
            }
            case DUP:
            {
                if (st_ptr >= STACK_SIZE) {
                    printf("Stack Overflow\n");
                    running = false;
                    break;
                }
                int top = this->stack[this->st_ptr - 1];
                this->stack[this->st_ptr] = top;
                this->st_ptr++;
                break;

            }
            case CALL: // CALL addr
            { 
                // get target jump addr
                int target = *(int *)(this->inst_ptr + 1);

                // calc return addr (curr location + 5 bytes)
                // store this as an OFFSET from start of prog
                int return_addr = (int)(this->inst_ptr - this->program) + 5;

                // push to RETURN STACK
                this->ret_stack[this->rst_ptr] = return_addr;
                this->rst_ptr++;

                // jump
                this->inst_ptr = this->program + target;
                continue;
            }

            case RET: // RET
            { 
            // check if we have anywhere to return to
            if (this->rst_ptr == 0) {
                printf("Error: Stack underflow on RET\n");
                running = false;
                break;
            }

            // pop from RETURN STACK
            this->rst_ptr--;
            int ret_addr = this->ret_stack[this->rst_ptr];

            // jump back
            this->inst_ptr = this->program + ret_addr;
            continue;
            }
            default:
                printf("Unknown Opcode %x\n", opcode);
                running = false;
                break;

        }

        // move to next instruction (if not stopping)
        if(running){
            this->inst_ptr++;
        }


    }

}