; Test 3: Loop - Sum from 1 to 10
; Initialize: sum=0, counter=1
; Expected result: 55

PUSH 0
STORE 0     ; PC 5: memory[0] = sum

PUSH 1
STORE 1     ; PC 15: memory[1] = counter

; LOOP START (Byte 20)
LOAD 0      ; PC 20: Load sum
LOAD 1      ; PC 25: Load counter
ADD         ; PC 30: sum + counter
STORE 0     ; PC 31: Store new sum

; Increment counter
LOAD 1      ; PC 36
PUSH 1      ; PC 41
ADD         ; PC 46
STORE 1     ; PC 47: counter++

; Check if counter <= 10
LOAD 1      ; PC 52
PUSH 11     ; PC 57
CMP         ; PC 62
JZ 73       ; PC 63: If counter >= 11, Jump to END (Byte 73)

; Continue loop
JMP 20      ; PC 68: Jump back to LOOP START (Byte 20)

; END (Byte 73)
LOAD 0      ; PC 73: Load final sum
HALT        ; PC 78