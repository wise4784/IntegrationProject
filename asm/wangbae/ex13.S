		.data
fmt:	.asciz "23th = %d\n"

		.text
		.global main

main:	stmfd sp!, {lr}
		mov r3, #3
		mov r2, #1
		mov r0, #0
		b loop
loop:	cmp r0, #21
		bge endfor
		add r1, r2, r3
		mov r2, r3
		mov r3, r1
		add r0, r0, #1
		b loop
endfor:	ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
