2000  MOV SI, 3000    
2003  MOV DI, 4000    
2006  MOV AX, [SI]    
2008  INC SI
2009  INC SI          
200A  ADD AX, [SI]    
200C  MOV BX, AX      
200E  MUL BX          
2010  MOV [DI], AH    
2012  INC DI
2013  MOV [DI], AL    
2015  HLT

Input 
3000: 02h    
3002: 03h    

Output
4000: 00h    
4001: 19h    
