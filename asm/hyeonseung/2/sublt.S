		.data
fmt:	.asciz "r1 = %d\n"

		.text
		.globl main

main:	stmfd sp!, {lr}
		mov r1, #34
		mov r2, #77
		mov r3, #37
		cmp r3, r2
		bgt print
		sublt r1, r1, #1
print:  ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr

