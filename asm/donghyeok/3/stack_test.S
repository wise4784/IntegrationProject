		.data
fmt:	.asciz "23번째 = %d\n"

		.text
		.globl	main

func:	push	{r11,lr}
		add		r11, sp, #4
		sub		sp, sp, #92

funins:	mov		r3, r0
		mov		r2, #2
		mov		r1, #0
		mov		r5, #0
		
		mov		r0, #1
		sub		r4, sp, r11
		str		r0, [r11, r4]

		mov		r0, #3
		add		r5, r4, #4
		str		r0, [r11, r5]

loop:	cmp		r2, r3
		bgt		funend

		sub		r5,	r2, #2
		lsl		r5, #2
		add		r5, r5, r4
		ldr		r0, [r11,r5]

		sub		r5, r2, #1
		lsl		r5, #2
		add		r5, r5, r4
		ldr		r1,	[r11,r5]

		add		r0, r0, r1
		mov		r5, r2
		lsl		r5, #2
		add		r5, r5, r4
		str		r0,	[r11,r5]

		add		r2, r2, #1
		b		loop

funend:	sub		sp,	r11, #4
		pop		{r11, pc}
		

main:	push	{r11, lr}
		add		r11, sp, #4

		mov		r0, #22
		bl		func

		mov		r1, r0
		ldr		r0, =fmt
		bl		printf

		mov 	r0, #0
		sub		sp, r11, #4
		pop		{r11, pc}
