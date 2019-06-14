

			.text
			.global main

enqueue:	stmfd sp!, {lr}
			ldmfd sp!, {lr}
			mov pc, lr

main:		stmfd sp!, {lr}
			mov r0, #0
			ldmfd sp!, {lr}
			mov pc, lr

