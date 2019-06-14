		.data
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

fmt:	.asciz	"res: %d\n"

		.text
		.global main

fibo:	stmfd sp!, {lr}
		ldr r5, =arr
		mov r3, #0
		mov r0, #1
		str r0, [r5,r3]
		mov r1, #3
		add r3, r3, #4
		str r1, [r5,r3]
		mov r4, #88

loop:	mov r2, r1
		add r1,r1,r0
		mov r0,r2
		add r3,r3,#4
		str r1, [r5,r3]
		cmp r3, r4
		blt	loop
		mov r0,r1
		ldmfd sp!, {lr}
		mov pc, lr	

main:	stmfd sp!, {lr}
		bl fibo
		ldr r1,	=arr
		ldr r1,	[r1,#88]
		ldr r0,	=fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
