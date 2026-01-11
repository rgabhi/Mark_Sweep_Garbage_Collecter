; Test 5: Fibonacci sequence (iterative)
; Calculate 10th Fibonacci number -> 55

; Initialize: prev = 0, curr = 1, counter = 2
PUSH 0      ; Bytes 0-4
STORE 0     ; Bytes 5-9

PUSH 1      ; Bytes 10-14
STORE 1     ; Bytes 15-19

PUSH 2      ; Bytes 20-24
STORE 2     ; Bytes 25-29

; Loop START (Byte 30)
LOAD 0      ; Bytes 30-34 (prev)
LOAD 1      ; Bytes 35-39 (curr)
ADD         ; Byte 40     (next)

; Shift values
LOAD 1      ; Bytes 41-45
STORE 0     ; Bytes 46-50 (prev = curr)
STORE 1     ; Bytes 51-55 (curr = next)

; Increment counter
LOAD 2      ; Bytes 56-60
PUSH 1      ; Bytes 61-65
ADD         ; Byte 66
STORE 2     ; Bytes 67-71

; Check if counter < 10
LOAD 2      ; Bytes 72-76
PUSH 10     ; Bytes 77-81
CMP         ; Byte 82
JNZ 30      ; Bytes 83-87 (Jump back to Byte 30)

; End
LOAD 1      ; Bytes 88-92
HALT        ; Byte 93