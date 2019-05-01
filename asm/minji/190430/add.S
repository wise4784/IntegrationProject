//data 영역
		.data
fmt:	.asciz	"r1, r2, r3 = %d, %d, %d\n"

//text 영역
		.text
		.global	main

//****assembly 영역에서는 변수가 없다********
main:
		stmfd	sp!, {lr}
		mov		r3, #77
		mov		r2, #33
		add		r1, r2, r3	//r1 = r2 + r3
		ldr		r0, =fmt
		bl		printf		//링커가 printf를 연결해 준다.
		mov		r0, #0
		ldmfd	sp!, {lr}
		mov		pc, lr

// data, code(text), heap, stack -> 메모리 영역
