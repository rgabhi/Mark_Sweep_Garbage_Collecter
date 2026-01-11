; Test 8: Conditional branching
; Find maximum of two numbers: max(15, 23)

PUSH 15
STORE 0     ; PC 5: a = 15

PUSH 23
STORE 1     ; PC 15: b = 23

; Compare a < b
LOAD 0      ; PC 20
LOAD 1      ; PC 25
CMP         ; PC 30

JZ 46       ; PC 31: If false (a >= b), jump to ELSE (Byte 46)

; THEN branch
LOAD 1      ; PC 36: Load b
JMP 51      ; PC 41: Jump to END (Byte 51)

; ELSE branch (Target 46)
LOAD 0      ; PC 46: Load a

; END (Target 51)
HALT        ; PC 51