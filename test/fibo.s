.equ print, 0x6b
.equ prchr, 0x0
.equ exit, 0x11
mov r0,#1
mov r1,#1
mov r2,#1 @firstnum 
mov r3,#1 @secondnum
mov r4,#1 @counter
mov r5,#0 @temp
mov r6,#6 @limit
mov r1,r2
swi print
swi prchr

loop:
cmp r4,r6
beq lalal
add r4,r4,#1
mov r5,r3
add r3,r3,r2
mov r2,r5
mov r1,r3
swi print
swi prchr
b loop

lalal:
swi exit
