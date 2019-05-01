		.data

fmt:	.asciz "23번째 = %d\n"
n:		.asciz "\n"
r:		.asciz "\r"
arr:	.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0

		.text
		.globl main

func:	stmfd 	sp!, {lr}
		mov 	r0, #0
		mov		r1, #0
		ldr		r2, =arr
		mov		r3, #0
		mov		r4, pc	
		cmp		r3, #0
		moveq	r0, #1
		cmp		r3,	#4
		moveq	r0,	#3
		subgt	r3, r3, #4
		ldrgt	r0,	[r2,r3]
		subgt	r3,	r3, #4
		ldrgt	r1, [r2,r3]
		addgt	r0, r0, r1
		addgt	r3, r3, #8
		str		r0,	[r2,r3]
		cmp		r3,	#88
		addlt	r3,	r3, #4
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
		
