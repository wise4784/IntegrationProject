		.equ	link, 0
		.equ	data, 4
		.equ	NULL, 0
		
		.data
fmt: 	.asciz "r1(queue data) = %d\n"

		.text
		.globl	main

get_node:	stmfd	sp!, {lr}
			mov	r0, #8
			bl malloc
			mov	r5,	#0
			str	r5, [r0]
			ldmfd sp!, {lr}
			mov pc, lr

enqueue:	push {fp,lr}
			add fp, sp, #4
		 	sub sp, sp, #12
		 	str r0, [fp,#-8]
		 	str r1,	[fp,#-12]
			ldr r4, [fp,#-8]
			ldr r8, [fp,#-12]
			ldr r6, [r4]
			cmp r6, #0
			bne	recurs
			bl	get_node
			str r0, [r4]
			str r8, [r0,#4]
			b fin
recurs:		mov	r0,	r6
			ldr r1,	[fp, #-12]
			bl	enqueue
fin:		sub sp,	fp,	#4
			pop	{fp,pc}


print:      push {r11,lr}
            add r11, sp, #4
            sub sp, sp, #4
            str r1, [r11, #-8]
            mov r2, r1

print_loop: cmp r2, #0
            beq print_fin
			str r2,[r11,#-8]
            ldr r0, =fmt
            ldr r1, [r2,#4]
            bl printf
			ldr r2, [r11,#-8]
			ldr r2, [r2]
            b print_loop

print_fin:  sub sp, r11,#4
            pop {r11,pc}



main:	push	{fp,lr}
		add	fp,	sp,	#4
		sub	sp,	sp,	#8
		mov	r3,	#0
		str	r3,	[fp,#-8]
		mov	r1,	#10
		sub	r0,	fp,	#8
		bl	enqueue
		mov	r1,	#20
		sub	r0,	fp,	#8
		bl	enqueue
		#sub	r0,	fp,	#8
		#bl	pqueue
		ldr r1, [fp,#-8]
		bl print
		mov	r0, #0
		sub	sp,	fp, #4
		sub	sp, fp, #4
		pop	{fp, pc}

