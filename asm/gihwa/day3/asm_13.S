		.data
arr:	.word 1
		.word 3
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
		.word 0
				

fmt:	.asciz " result : %d \n"

		.text
		.globl main

make_series :	stmfd sp!, {lr}
				
i:		.word 84
				
				ldr r5, =arr
				mov r4, #0

for :			ldr r2, [r5,r4]
				add r4, #4
				ldr r3, [r5,r4]
				add r1, r2, r3
				add r4, #4
				str r1, [r5,r4]
				sub r4, #4				

				ldr r6, =i
				ldr r6, [r6]
				cmp r4, r6
				blt for
				ldr r0, =fmt
				bge printf

				ldmfd sp!, {lr}
				mov pc, lr

main:	stmfd sp!, {lr}
		
		bl make_series		

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
