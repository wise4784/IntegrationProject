		.data
fmt:	.asciz	"res: %d\n"

		.text
		.global main

fibo:	stmfd sp!, {lr}
		mov r3, #0
		mov r0, #1
		mov r1, #3
		mov r4, #21
		
loop:	mov r2, r1
		add r1,r1,r0
		mov r0,r2
		add r3,r3,#1
		cmp r3, r4
		blt	loop
		mov r0,r1
		ldmfd sp!, {lr}
		mov pc, lr	

main:	stmfd sp!, {lr}
		bl fibo
		mov r1, r0
		ldr r0,=fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
