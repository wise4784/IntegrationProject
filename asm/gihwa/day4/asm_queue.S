			.data
fmt:		.asciz "ad = %x\n"

			.text
			.globl main

enqueue:	stmfd sp!, {lr}
			#sub sp, sp, #8
			mov r0, #8
			bl malloc
			mov r3, r0
loop:
			#data 2 store
			mov r2, #2
			str r2, [r0, #4]
			
			ldmfd sp!, {lr}
			mov pc, lr
			r1

printqueue:
			stmfd sp!, {lr}
			

			ldmfd sp!, {lr}
			mov pc, lr



main:		stmfd sp!, {lr}
			
						
			bl enqueue
			ldr r1, [r0, #4]
			ldr r0, =fmt
			bl printf
			
			mov r0, #0
			ldmfd sp!, {lr}
			mov pc, lr
