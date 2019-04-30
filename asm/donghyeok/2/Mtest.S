		.macro MTEST
		mov r1, #7
		.endm

		.macro REV 	a,b
		mov r4, \a
		mov \a, \b
		mov \b, r4
		.endm

		.data
fmt:	.asciz "r1, r2, r3 = %d, %d, %d\n"

		.text
		.globl main
main: 	stmfd sp!, {lr}
		MTEST
		mov r2, #3
		mov r3, #4
		ldr r0, =fmt
		bl printf
		MTEST
		mov r2, #3
		mov r3, #4
		REV r2, r3
		ldr r0, =fmt
		bl printf
		mov r0,#0
		ldmfd sp!, {lr}
		mov pc, lr
