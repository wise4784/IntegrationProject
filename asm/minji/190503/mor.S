			.data
fmt:		.asciz	"순차진행 테스트\n"
fmt2:		.asciz	"r0 = %d\n"
	
			.text
			.global	main

print:		push	{fp, lr}
			ldr		r0, =fmt2
			bl		printf

addr:		mov		r1, #1
			mov		r2, #9
			add		r0, r1, r2

main:		stmfd	sp!, {lr}
			ldr		r0, [sp]
			bl		addr
			bl		print
			ldmfd	sp!, {lr}
			mov		pc, lr
