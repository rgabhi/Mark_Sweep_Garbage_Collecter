; Test 7: Memory Load/Store operations
; Store multiple values and retrieve them
; Expected result: 100

; Store values in memory
PUSH 10
STORE 0     ; memory[0] = 10

PUSH 20
STORE 1     ; memory[1] = 20

PUSH 30
STORE 2     ; memory[2] = 30

PUSH 40
STORE 3     ; memory[3] = 40

; Load and compute: (mem[0] + mem[1]) + (mem[2] + mem[3])
LOAD 0
LOAD 1
ADD         ; 30

LOAD 2
LOAD 3
ADD         ; 70

ADD         ; 100
HALT        ; Result: 100