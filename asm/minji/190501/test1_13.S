		.data				//data 영역에 전역변수 선언
arr:	.word	1			//
		.word	3
		.skip	21 * 4		//21개의 공간을 4 byte만큼 할당한다.
		
fmt:	.asciz	"r1 = %d\n"

		.text				//기계어는 무조건 text
		.global main		//text, data, heep, text
							//text 영역에 코드가 올라가 있다.
							//text 영역에 main이 있다(.global main)
#malloc -> heep 영역에서 임의로 가져온다.
#단, heep은 표시 할 수 없다.
#기계어 분석 -> 포인터 개념 바로 잡기
#
#
#r0: return 보유
#r7: system call 보유
print:	stmfd	sp!, {lr}
		ldr		r0, =fmt
		bl		printf
		mov		r0, #0
		ldmfd	sp!, {lr}
		mov		pc, lr

#int	find(int num)	<< 23
find:	stmfd	sp!, {lr}
		mov		r1, r0
		mov		r2, #2
loop:	cmp		r1, r2
		ble		fin
		ldr		r3, =arr
		sub		r8, r2, #1
		lsl		r8, #2
		ldr		r4, [r3, r8]
		sub		r5, r2, #2
		lsl		r5, #2
		ldr		r6, [r3, r5]
		add		r0, r4, r6
		mov		r7, r2
		lsl		r7, #2
		str		r0, [r3, r7]
		add		r2, r2, #1
		b		loop
fin:	ldmfd	sp!, {lr}
		mov		pc, lr

main:	stmfd	sp!, {lr}
		mov		r0, #23
		bl		find
		mov		r1, r0
		bl		print
		ldmfd	sp!, {lr}
		mov		pc, lr

#메모리 구조
#text(code): 실제 프로그램 코드가 저장되어 있는 영역
#data: 전역변수와 정적변수가 저장되는 영역
#stack: 함수의 호출과 관계되는 지역 변수와 매개변수가 저장되는 영역
#heap: 사용자가 직접 관리할 수 있는 '그리고 해야만 하는' 메모리 영역
