		.data
i:		.word	0
j:		.word	1
fmt:	.asciz	"Hello, %d\n"
fmt2:	.asciz	"Hello, %s\n"
ch:		.byte	'A', 'B', 0
array:	.word	0, 1, 2, 3, 4

		.text
		.global	main

main:	stmfd	sp!, {lr}
		ldr		r1, =i		//i의 주소값(메모리 번지수)를 r0에 받는다.
		ldr		r1, [r1]	//꺽쇄에 있는 인자의 값을 받아온다. r1 = 0
		ldr		r0, =fmt	//fmt의 주소값을 r0에 받는다
		bl		printf		//bl (branch + load): printf 함수에 링크 간 후 lr에 복귀 주소 저장
		ldr		r1, =j		//j의 주소값을 r1에 받는다.
		ldr		r1, [r1]	//꺽쇄에 있는 인자 값을 받아온다.	r1 = 1
		ldr		r0, =fmt	//fmt의 주소값을 r0에 받는다.
		bl		printf		//printf 함수에 분기점을 두고 복귀주소도 같이 저장한다.
		ldr		r1, =ch		//r1 <- ch 주소값
		ldr		r0, =fmt2	//r0 <- fmt2 주소값
		bl		printf		//분기+복귀주소
		ldr		r1, =array
		ldr		r0, =fmt
		mov		r0, #0		//r0 = 0
		ldmfd	sp!, {lr}	
		mov		pc, lr
/*

메모리 연산과 같은 것 = 포인터 연산

*/
