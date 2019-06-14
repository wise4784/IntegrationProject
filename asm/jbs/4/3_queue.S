		.data
arr:	.word 10
		.word 20
		.word 30
		.word 40


		.text
		.globl main

enqueu:	stmfd sp!, {lr}
		sub sp, sp, #40
		add r0, sp, #36
		mov r1, #40
		mov r2, #0
		mov r4, #0
		mov r0, =r4

loop:	cmp r1, r2
		ble fin
qoop:	cmp 		

		ldmfd sp!, {lr}
		mov pc, lr
fin:	ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		mov r0, #0
		bl enqueu

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
