		.data

arr:	.word	1
		.word	3
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
		
fmt:	.asciz "r1 = %d\n"

		.text
		.global main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		ldr r3, =arr
		mov r2, #0
		ldr r1, [r3, r2] 


		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr

/*
		ldr r3, =arr
        mov r2, #4
        ldr r0, [r3, r2]
		bl print
*/
