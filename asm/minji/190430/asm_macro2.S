		.macro MTEST
		mov		r1, #7
		.endm

		.macro	REV	a, b
		mov r4, \a
		mov	\a, \b
		mov \b, r4
		.endm

		.data
fmt:	.asciz	"r1, r2, r3 = %d, %d, %d\n"

		.text
		.global	main

main:	stmfd	sp!, {lr}
		mov		r1, #1
		mov		r2, #123
		mov		r3, #22
		MTEST
		ldr		r0, =fmt
		bl		printf
		mov		r2, #123	//bl을 통해 printf 함수를 사용하고 나면
		mov		r3, #22		//r2, r3 값을 다시 세팅해야 한다. (레지스터 사용할 수 있으니)
		REV		r2, r3
		ldr		r0, =fmt
		bl		printf
		mov		r0, #0
		ldmfd	sp!, {lr}
		mov		pc, lr
