		.data
/*
arr:	.word	1
		.word	3
		.word	4
*/	
fmt:	.asciz "r1 = %d\n"

		.text
		.global main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		mov r3, #1
		mov r2, #3
		str r4, r3
		str r5, r2
		add r1, r2, r3
		bl print
		ldr r3, r4
		ldr r2, r5
		mov r3, r2
		mov r2, r1
		add r1, r2, r3
		bl print
		mov r0, #0
        ldmfd sp!, {lr}
        mov pc, lr
		
		
