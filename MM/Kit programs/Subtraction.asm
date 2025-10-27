2000 MOV SI, 3000       ; SI <- immediate 0x3000 (address of first 16-bit word)
2003 MOV DI, 4000       ; DI <- immediate 0x4000 (destination base for borrow flag + result)
2006 MOV AX, [SI]       ; AX <- word at memory[SI] (load word at address 0x3000)
2008 INC SI             ; SI = SI + 1 (advance by 1 byte)
2009 INC SI             ; SI = SI + 1 (advance another byte; SI now points to 0x3002)
200A MOV BX, [SI]       ; BX <- word at memory[SI] (load word at address 0x3002)
200C CMP AX, BX         ; compare AX and BX (sets CF=1 if unsigned AX < BX)
200E JC 2017            ; jump if carry flag = 1 (borrow / AX < BX) -> handle borrow
2010 SUB AX, BX         ; AX = AX - BX (no-borrow case: compute result)
2012 MOV [DI], 0000     ; store 0000 at [DI] to indicate no borrow
2015 JMP 2022           ; skip borrow-handling block
2017 MOV CX, AX         ; CX = AX (save original AX)
2019 MOV AX, BX         ; AX = BX
201A MOV BX, AX         ; BX = AX (duplicates AX; likely intended MOV BX, CX to complete a swap)
201B SUB AX, BX         ; AX = AX - BX (with current code this yields 0 because AX==BX)
201F MOV [DI], 0001     ; store 0001 at [DI] to indicate borrow
2022 INC DI             ; DI = DI + 1 (advance to result word)
2023 MOV [DI], AX       ; store AX (result) at [DI]
2025 HLT                ; halt

Input:
    [3000] = 0007
    [3002] = 0003

Output:
    [4000] = 0000
    [4001] = 0004