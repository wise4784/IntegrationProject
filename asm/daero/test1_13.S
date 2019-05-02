		.data
fmt:	.asciz "23th num = %d\n"
		.text
		.global main

f23th:	stmfd sp!, {lr}

		add r4, r1, r2

		mov r1, r2
		mov r2, r4
		add r3, #1

		mov r5, #23
		cmp r5, r3
		bne f23th		

		mov r1, r4
		ldr r0, =fmt
		bl printf

		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
	
		mov r1, #1
		mov r2, #3
		mov r3, #2

		bl f23th

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
