DATA SEGMENT
    ARRAY   DW 1111H, 5555H, 2222H, 8888H, 4444H
    COUNT   DW 05H
    LARGEST DW ?
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX

    LEA SI, ARRAY   
    MOV CX, COUNT   
    MOV AX, [SI]   
    
    DEC CX          
    JZ EXIT         
    
UP:
    ADD SI, 2       
    CMP AX, [SI]    
    JGE NO_CHANGE   
    MOV AX, [SI]     
    
NO_CHANGE:
    DEC CX
    JNZ UP
    
EXIT:
    MOV LARGEST, AX   
    MOV AH, 4CH
    INT 21H
CODE ENDS
END START