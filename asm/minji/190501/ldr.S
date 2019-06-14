		.data

arr:	.word	1
		.word	2
		.word	3
		.word	4
		.word	5

fmt:	.asciz	"r1 = 0x%x\n"

		.text
		.global	main

print:	stmfd	sp!, {lr}
		ldr		r0, =fmt
		bl		printf
		ldmfd	sp!, {lr}
		mov		pc, lr

main:	stmfd	sp!, {lr}
		ldr		r3, =arr		//r3에 arr의 시작 주소값이 들어가있다.
		mov		r2, #8			//word size가 32bit=4byte -> 2칸씩 건너뛰어야 하므로 8로 세팅함
		ldr		r1, [r3, r2]	//r1 := *(r3+r2)
		bl		print
		mov		r0, #0
		ldmfd	sp!, {lr}		
		mov		pc, lr
