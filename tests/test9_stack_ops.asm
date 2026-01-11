; Test 9: Stack manipulation operations
; Tests: PUSH, POP, DUP
; Expected result: 25

PUSH 10
PUSH 20
PUSH 30     ; Stack: [10, 20, 30]

POP         ; Stack: [10, 20]

DUP         ; Stack: [10, 20, 20]

ADD         ; Stack: [10, 40]

PUSH 5
SUB         ; Stack: [10, 35]

POP         ; Stack: [10]

PUSH 15
ADD         ; Stack: [25]

HALT        ; Result: 25