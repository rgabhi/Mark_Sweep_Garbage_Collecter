; Test 1: Basic Arithmetic Operations
; Tests: PUSH, ADD, SUB, MUL, DIV
; Expected result: 42

; Calculate: ((10 + 5) * 3) - 3 = 42
PUSH 10
PUSH 5
ADD         ; Stack: [15]
PUSH 3
MUL         ; Stack: [45]
PUSH 3
SUB         ; Stack: [42]
HALT