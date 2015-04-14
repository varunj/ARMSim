arr: .skip 24
.equ print, 0x6b
.equ prchr, 0x0
tabSpace: .asciz "\t"
newLine: .asciz "\n"
newLine2: .asciz "\n "

LDR r4, =arr
MOV r0, #1
MOV r3, #0 @counter 
MOV r2, #6 @limit
LOOP:
	STR r3, [r4,r3,LSL#2]
	ADD r3, r3, #1
	CMP r3, r2
	BLT LOOP
MOV r1, #0 @sum
MOV r3, #0 @counter
LOOP2:
	LDR r6, [r4,r3,LSL#2]
	ADD r1, r1, r6
	ADD r3, r3, #1
	CMP r3, r2
@	SWI print
@	SWI prchr
	BLT LOOP2
SWI 0x11