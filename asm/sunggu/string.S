		.data

carr:	.asciz	"HelloARM"
fmt:	.asciz	"r1 = %c\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		ldr r2, =carr		//r2에 배열의 주소를 넣고
		ldrb r1, [r2, #5] 	//그 주소로부터 5번째 값인 A가 출력된다.
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
