		.data
fmt: 	.asciz "num = %d\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

get_node:	stmfd sp!, {lr}

			mov r0, #4
			bl malloc

			ldmfd sp!, {lr}
			mov pc, lr

enqueue:	push {fp, lr}

			mov r4, r1

			add fp, sp, #4
			sub sp, sp, #4			

			bl get_node

			sub r5, fp, #8
			str r5, [r0]
			str r0, [r4]
		
fin:		pop {fp, lr}
			mov pc, lr

main:	stmfd sp!, {lr}

		mov r4, #0
		push {r4}
		add fp, sp, #4
		
		mov r1, #3
		bl enqueue

		mov r1, #4
		bl enqueue	

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
