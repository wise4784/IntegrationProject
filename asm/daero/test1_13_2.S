		.data

arr:	.word	1
		.word	3
		.skip	21*4

fmt:	.asciz "0x%d\n"
		
		.text
		.global main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

f23th:	stmfd sp!, {lr}
		
		ldr r4, =arr
		ldr r5, [r4, r1]
		ldr r6, [r4, r2]
		ldr r8, [r4, r3]

		add r8, r5, r6
		str r8, [r4, r3]

		add r1, r1, #4
		add r2, r2, #4
		add r3, r3, #4
		add r0, r0, #1

		mov r9, #23
		cmp r9, r0
		bne	f23th

		sub r3, r3, #4

		ldr r1, [r4, r3]
		ldr r0, =fmt
		bl printf

		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		
		mov r0, #2
		mov r1, #0
		mov r2, #4
		mov r3, #8
		
		bl f23th

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
