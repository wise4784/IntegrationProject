		.data

fmt:	.asciz "23번째 = %d\n"

		.text
		.globl main

func:	stmfd 	sp!, {lr}
		mov 	r0, #0
		mov 	r1, #0
		mov		r2, #0
		mov		r3, #0
		mov		r4,	pc
		cmp		r3, #0
		moveq 	r0,	#1
		cmp		r3,	#1
		moveq	r0,	#3
		addgt	r0, r1, r2
		mov		r2, r1
		mov 	r1, r0
		cmp		r3,	#22
		addlt	r3,	r3, #1
		movlt	pc, r4
		ldmfd	sp!, {lr}
		mov		pc, lr

main:	stmfd sp!, {lr}
		bl	func
		mov r1, r0
		ldr r0, =fmt
		bl	printf
		mov	r0, #0
		ldmfd	sp!, {lr}
		mov 	pc, lr
		
