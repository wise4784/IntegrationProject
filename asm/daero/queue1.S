		.equ	link, 0
		.equ	data, 4
		.equ 	NULL, 0

		.data
fmt:	.asciz "r1(queue data) = %d\n"

		.text
		.globl main

pqueue:	push {fp, lr}
		add fp, sp, #4

		mov r4, #0

		sub r5, fp, #0

pstt: 	add r5, r5, #4
		ldr r6, [r5]

		cmp r6, r4
		bne	pstt

ploop:	sub r5, r5, #4

		cmp r5, fp
		beq pfin
		
		ldr r1, [r5]
		ldr r1, [r1]

		ldr r0, =fmt
		bl printf

		b ploop

pfin:	sub sp, fp, #4
		pop {fp, pc}

get_node:	stmfd sp!, {lr}

			mov r0, #4
			bl malloc

			ldmfd sp!, {lr}
			mov pc, lr

enqueue:	push {fp, lr}

			add fp, sp, #4
			mov r4, r1

			bl get_node

			str r0, [fp, #4]

			str r4, [r0]
			
			sub sp, fp, #4
			pop {fp, pc}

main:		push {fp, lr}
			
			add fp, sp, #4
			sub sp, sp, #8

			mov r3, #0
			str r3, [fp, #-8]

			mov r1, #10
			bl enqueue

			sub sp, sp, #4
			mov r1, #11
			bl enqueue

			sub sp, sp, #4
			mov r1, #12
			bl enqueue

			bl pqueue

			mov r0, #0
			sub sp, fp, #4
			pop {fp, pc}
