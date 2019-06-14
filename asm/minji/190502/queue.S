			.data
arr:		.word	1
			.word	2
			.word	3
			.word	4
			.word	5
			.word	6
			.word	7

fmt:		.asciz	"data = %d "
fmt2:		.asciz	"\n"

			.text
			.global main

print:		stmfd	sp!, {lr}
			ldr		r0, =fmt
			bl		printf
			ldmfd	sp!, {lr}
			mov		pc, lr

enqueue:	stmfd	sp!, {lr}
#			push	lr
			ldmfd	sp!, {lr}
			mov		pc, lr

#queue_data: 1, 2, 3, 4, 5, 6, 7
#input 1->2->3->4->5->6->7	1부터 순서대로 넣어서
#output	1->2->3->4->5->6->7	1부터 순서대로 저장

main:		stmfd	sp!, {lr}
			mov		r2, #7
			lsl		r2, #2			//r2 = 28
			sub		sp, sp, r2		//sp = sp - 0x1c
			ldr		r1, =arr
			str		r1, [sp, r2]
			ldrb	r2, [r1]
#			bl		print

			mov		r0, #0
			ldmfd	sp!, {lr}
			mov		pc, lr
