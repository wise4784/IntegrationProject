		.data
fmt:	.asciz	"r1 = %d\n"

		.text
		.global	main

main:	stmfd	sp!, {lr}
		mov		r1, #34
		mov		r2, #77
		mov		r3, #28
		cmp		r2, r3		//compare
		ble		print		//bl + ...e?
		subgt	r1, r1, #1	//sub+ ...gt?
print:	ldr		r0, =fmt
		bl		printf
		mov		r0, #0
		ldmfd	sp!, {lr}
		mov		pc, lr

//cmp	r2, r3	 r2와 r3를 비교 결과를 cpsr에 저장한다. (조건 관련된 애들은 cpsr가 관리한다)
		// r2가 더 크기 때문에
		// 다음 ble가 동작하지 않는다. (brach less equall)
		// ble: 조건부 call (r2랑 r3가 같지 않으므로 print 호출하지 않는다.

//subgt	r1, r1, #1	(sub + greater than) cmp 결과가 greater than이면 sub 수행
//sublt r1, r1, #1	(sub + less than)	 cmp 결과가 less than이면 sub 수행



! ble, bgt, blt addgt, addlt, subgt, subgt 등등 CMP 명령을 전제로 동작하게 된다
! 이걸 이용해서 for문 알고리즘을 구현할 수 있다.


