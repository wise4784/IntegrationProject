		.data
fmt:	.asciz	"r1 = 0x%x\n"

		.text
		.global main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		mov r3, #0
		mov r2, #0
		cmp r2, r3
		mrs r1, cpsr
		bl print
	
		mov r3, #0
		mov r2, #2
		cmp r3, r2
		mrs r1, cpsr
		bl print

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
