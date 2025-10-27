DATA SEGMENT
    ARRAY   DW 5
            DW 20, 8, 55, 2, 16
 DATA ENDS
 CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
 START:
    MOV AX, DATA
    MOV DS, AX
    LEA SI, ARRAY
    MOV CX, [SI]
    DEC CX
    
OUTER_LOOP:
    LEA SI, ARRAY
    MOV DX, [SI]
    DEC DX
    ADD SI, 2
    
INNER_LOOP:
    MOV AX, [SI]
    ADD SI, 2
    CMP AX, [SI]
    JC NO_SWAP
    
    XCHG AX, [SI]
    SUB SI, 2
    XCHG AX, [SI]
    ADD SI, 2
    
NO_SWAP:
    DEC DX
    JNZ INNER_LOOP
    
    DEC CX
    JNZ OUTER_LOOP
    
    MOV AH, 4CH
    INT 21H
    
CODE ENDS
END START