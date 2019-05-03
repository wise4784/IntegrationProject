		.macro	SHIFT	a,b
		.if	\b < 0
		mov \a, \a, asr #-\b
		.else
		mov \a, \a, asr #\b
		.endif
		.endm

		.data
fmt:	.asciz	"Hello,%d, %d\n"

		.text
		.globl	main

main:	stmfd sp!,	{lr}
		ldr r0,=fmt
		mov r1, #1
		SHIFT r1, 1
		mov r2,#2
		SHIFT r2, 1
		bl printf

		mov r1, #300
		mov r2, #33
		SHIFT r2, 3
		SHIFT r1, -6
		ldr r0,=fmt
		bl printf

		mov r0,#0
		ldmfd sp!,	{lr}
		mov pc, lr
