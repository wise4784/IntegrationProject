			.data
parent:		.asciz "r0 = %u, parent\n"
child:		.asciz	"r0 = %u, child\n"

			.text
			.globl	main
	
main:		push {r7, lr}
			mov r7, #2
			swi	#0
			mov r1, r0
if:			subs r2, r0, #0
			ldrne r0, =parent
			ldreq r0,=child
			bl printf
			mov r0, #0
			pop {r7, pc}
