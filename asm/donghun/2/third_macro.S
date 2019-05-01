	.macro SHIFT	a, b
	.if \b < 0
	mov \a, \a, asr #-\b
	.else
	mov \a, \a, asr #\b
	.endif
	.endm

	.data
fmt:	.asciz	"Hello, %d, %d\n"

	.text
	.global main

main:	stmfd sp!, {lr}
	mov r1, #300
	mov r2, #33
	SHIFT r2, 3
	SHIFT r1, -6
	ldr r0, =fmt
	bl printf
	mov r0, #0
	ldmfd sp!, {lr}
	mov pc, lr
