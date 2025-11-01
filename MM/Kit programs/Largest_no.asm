400 MOV SI, 500
403 MOV CX, [SI]    
405 ADD SI, 2       
406 MOV AX, [SI]    
408 DEC CX          
409 JZ 415              
40B ADD SI, 2       
40C CMP AX, [SI]    
40E JGE 412         
410 MOV AX, [SI]        
412 DEC CX
413 JNZ 40B             
415 MOV [520], AX   
418 HLT

; Input:
; 500H : Count N (e.g., 05)
; 502H : First number
; 504H : Second number
; ...
;
; Output:
; 520H : The largest number