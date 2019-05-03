		.data
fmt: 	.asciz "num = %d\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

enqueue:	stmfd sp!, {lr}

			mov r4, r1

			mov r0, #4
			bl malloc
			
			mov r5, r0
			str r4, [r5, #0]
			
			mov r1, r4
			bl print

			mov r0, r5

			ldmfd sp!, {lr}
			mov pc, lr

main:	stmfd sp!, {lr}
		
		mov r1, #4
		bl enqueue	

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
