; Test 6: Nested function calls
PUSH 3          ; PC 0-4
PUSH 4          ; PC 5-9
CALL 21         ; PC 10-14 (Call add at byte 21)
CALL 23         ; PC 15-19 (Call square at byte 23)
HALT            ; PC 20
ADD             ; PC 21
RET             ; PC 22
DUP             ; PC 23
MUL             ; PC 24
RET             ; PC 25