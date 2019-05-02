		.data
fmt:	.asciz "23th = %d\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

find:	stmfd sp!, {lr}

		mov r1, r0
		mov r2, #2		

		add r3, sp, #4
		sub sp, sp, #92
		
		mov r4, #1
		mov r5, #3

		str r4, [r3, #0]
/*	
	str r5, [r3, #-4]
		
loop:	cmp r1, r2
		beq fin

		sub r8, r2, #1
		lsl r8, #2
		ldr r4, [r3, r8]

		sub r9, r2, #2
		lsl r9, #2
		ldr r5, [r3, r9]

		add r0, r4, r5
	
		mov r7, r2
		lsl r7, #2
		str r0, [r3, r7] 
		
		add r2, r2, #1
		b loop
*/
fin:	#add sp, sp, #92
		add sp, sp, #92
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		
		mov r0, #23
		bl find

#		mov r1, r0
#		bl print

		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr		
