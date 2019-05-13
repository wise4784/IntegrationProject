		.data
i:		.word	3
j:		.word	7
fmt:	.asciz	"Hello, %d\n"
fmt2:	.asciz	"Hello, %s\n"
ch:		.byte	'A', 'B', 0
array:	.word	0,1,2,3,4

		.text
		.global	main

main:	stmfd sp!, {lr}
		ldr r0,	=fmt
		ldr r1,	=i
		ldr r1, [r1]
		bl printf

		ldr	r0, =fmt
		ldr r1, =j
		ldr r1, [r1]
		bl printf

		ldr r0,=fmt2
		ldr r1,=ch
		bl printf	

		mov r0,#0
		ldmfd sp!, {lr}
		mov	pc,	lr
