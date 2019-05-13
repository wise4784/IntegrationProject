		.data				//data 영역에 전역변수 선언
		
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

#int	find(int *arr, int num)	<< 23
find:	stmfd	sp!, {lr}
		mov		r2, r0
		mov		r3, #2
loop:	cmp		r2, r3
		ble		fin
		sub		r6, r3, #1
		lsl		r6, #2
		sub		r6, r4, r6
		ldr		r5, [r1, r6]
		#ldr	r5, [r5]
		sub		r6, r3, #2
		lsl		r6, #2
		sub		r6, r4, r6
		ldr		r7, [r1, r6]
		#ldr	r7, [r7]
		add		r0, r5, r7
		mov		r6, r3
		lsl		r6, #2
		sub		r6, r4, r6
		str		r0, [r1, r6]
		add		r3, r3, #1
		b		loop
fin:	ldmfd	sp!, {lr}
		mov		pc, lr

#스택을 이용할 때 지역변수를 이용해서 함수를 사용할 때
#함수를 끊어내야 하므로 sp에서 값을 세팅해야한다.
main:	stmfd	sp!, {lr}
		mov		r2, #24
		lsl		r2, #2
		sub		sp, sp, r2
		sub		r2, r2, #4
		mov		r4, r2
		mov		r3, #1
		str		r3, [sp, r2]
		mov		r3, #3
		sub		r2, r2, #4
		str		r3, [sp, r2]
		mov		r1, sp
		mov		r0, #23
		bl		find
		mov		r1, r0
		bl		print
		add		sp, sp, #96
		mov		r0, #0
		ldmfd	sp!, {lr}
		mov		pc, lr

#메모리 구조
#text(code): 실제 프로그램 코드가 저장되어 있는 영역
#data: 전역변수와 정적변수가 저장되는 영역
#stack: 함수의 호출과 관계되는 지역 변수와 매개변수가 저장되는 영역
#heap: 사용자가 직접 관리할 수 있는 '그리고 해야만 하는' 메모리 영역
