		.data
fmt:	.asciz

		.text
		.global main

main:	stmfd sp!, {lr}
	
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
