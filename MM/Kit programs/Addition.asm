2000 MOV SI, 3000       ; 
2003 MOV DI, 4000       ; 
2006 MOV AX, [SI]       ; AX <- word at memory[SI] (loads [3000] into AX)
2008 INC SI             ; advance SI by 1 byte (first byte of next word)
2009 INC SI             ; advance SI by 1 byte (now SI points to 3002, the next word)
200A MOV BX, [SI]       ; BX <- word at memory[SI] (loads [3002] into BX)
200C ADD AX, BX         ; AX = AX + BX ; addition of the two words, sets carry flag if overflow
200E JNC 2015           ; if Carry Flag = 0 (no overflow) jump to 2015 to write 0000
2010 MOV [DI], 0001     ; carry occurred -> store 0001 at [4000] as carry/overflow indicator
2013 JMP 2018           ; jump to write the sum and finish
2015 MOV [DI], 0000     ; no carry -> store 0000 at [4000]
2018 INC DI             ; move DI to next word position (destination for the 16-bit sum)
2019 MOV [DI], AX       ; store 16-bit sum at [4001] (AX contains the result)
201B HLT                ; halt execution

Input:
    [3000] = 0005
    [3002] = 0010

Output:
    [4000] = 0000
    [4001] = 0015