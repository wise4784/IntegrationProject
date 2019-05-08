		.macro SHIFT	a, b
		.if \b <0
		mov \a, \a, asl #-\b
		.else
		mov \a, \a, asl #\b
		.endif
		.endm

		.data
fmt:	.asciz	"hellp, %d, %d \n"

		.text
		.global main

main:	stmfd sp!, {lr}

		mov r1, #3
		mov r2, #7
		SHIFT r2, 2
		SHIFT r1, -3
		ldr r0, =fmt
		bl printf
		mov r0, #0
		bl printf

		ldmfd sp!, {lr}
		mov pc, lr

