; Test 10: Complex computation with multiple features
; Calculate sum of squares from 1 to 5: 1^2 + ... + 5^2 = 55

; Initialize sum = 0, counter = 1
PUSH 0      ; Bytes 0-4
STORE 0     ; Bytes 5-9 (memory[0] = sum)

PUSH 1      ; Bytes 10-14
STORE 1     ; Bytes 15-19 (memory[1] = counter)

; LOOP START (Byte 20)
LOAD 1      ; Bytes 20-24
CALL 79     ; Bytes 25-29 (Call square function at Byte 79)

; Add square to sum
LOAD 0      ; Bytes 30-34
ADD         ; Byte 35
STORE 0     ; Bytes 36-40

; Increment counter
LOAD 1      ; Bytes 41-45
PUSH 1      ; Bytes 46-50
ADD         ; Byte 51
STORE 1     ; Bytes 52-56

; Check if counter < 6
LOAD 1      ; Bytes 57-61
PUSH 6      ; Bytes 62-66
CMP         ; Byte 67
JNZ 20      ; Bytes 68-72 (Jump back to Byte 20)

; End
LOAD 0      ; Bytes 73-77
HALT        ; Byte 78

; Square function (Byte 79)
DUP         ; Byte 79
MUL         ; Byte 80
RET         ; Byte 81