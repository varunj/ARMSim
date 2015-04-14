.equ print, 0x6b
.equ prchr, 0x0
.equ exit, 0x11
MOV r0,#1
MOV r2,#6 @limit r2
MOV r3,#0 @counter r3
L1:
CMP r3,r2
BGE ex
MOV r0,r3
BL fibo
MOV r1,r0
MOV r0,#1
SWI print
SWI prchr
ADD r3,r3,#1
B L1
fibo:
SUB sp,sp,#12 @adjust stack for 3 items
STR r4, [sp, #4]
STR r5, [sp, #8]
STR lr, [sp, #0]
MOV r4, r0
CMP r0,#2
BGE L2
MOV r0, #1
LDR r4,[sp,#4]
LDR r5,[sp,#8]
LDR lr,[sp,#0]
ADD sp, sp, #12
mov pc,lr
L2: 
SUB r0, r4, #1
BL fibo
MOV r5, r0
SUB r0,r4,#2
BL fibo
ADD r0,r0,r5
LDR r4, [sp, #4]
LDR r5, [sp, #8]
LDR lr, [sp, #0]
ADD sp, sp, #12
MOV pc, lr
ex:
SWI exit
