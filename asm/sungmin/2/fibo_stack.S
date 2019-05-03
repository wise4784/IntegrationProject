		.data
fmt:	.asciz	"arr[22]: %d\n"

		.text
		.global main	

main:	stmfd sp!, {lr}
		sub sp, sp, #92

		mov r0, #1
		mov r1, #3
		str r0, [sp,#0]
		str r1, [sp,#4]	
		mov r3, #4
		mov r4, #88

loop:	mov r2, r1
		add r1, r0, r1
		mov r0, r2
		add r3, r3, #4
		str r1, [sp,r3]
		cmp r3, r4
		blt loop


		ldr r0,=fmt
		ldr r1, [sp,#88]
		bl	printf

		add sp, sp, #92
		mov r0, #0

		ldmfd sp!, {lr}
		mov pc, lr
