; Test 2: Circle Area Calculation (from Lab spec)
; Calculate: A = πr² where r=5, π≈3
; Expected result: 75

PUSH 5      ; Push radius
DUP         ; Duplicate for squaring
MUL         ; Square radius (25)
PUSH 3      ; Push pi constant
MUL         ; Multiply (75)
HALT        ; Result is 75 on top of stack