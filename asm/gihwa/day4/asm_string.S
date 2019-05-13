		.data
	
carr:	.asciz	"HelloARM"
fmt:	.asciz "r1=%c\n"

		.text
		.globl main

print: 	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		ldr r2, =carr
		ldrb r1, [r2, #5]
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
