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
	.word	0
	.word	0

fmt:	.asciz	"23th = %d\n"

	.text
	.globl main

for:	cmp r0, #100
	movgt pc,lr
	add r1, r2, r3
	str r1, [r4, r0]
	mov r2, r3
	mov r3, r1
	add r0, r0, #4
	b for

main:	stmfd sp!, {lr}
	mov r0, #0
	mov r1,	#0
	mov r2, #1
	mov r3, #3
	ldr r4, =arr
	bl for
	ldr r0, =fmt
	bl printf
	mov r0, #0
	ldmfd sp!, {lr}
	mov pc, lr
