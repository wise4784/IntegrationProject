		.data
fmt:	.asciz "tmp->data =%d\n"

		.text
		.globl	main

get_node:	push {r1, r2, r3, lr}
			mov r0, #8
			bl malloc
			mov r1,#0
			str r1,[r0]
			pop {r1, r2, r3, pc}

enqueue:	push {r11, lr}
			add r11, sp, #4
			sub sp, sp, #4

			str r0, [r11,#-8]
			ldr r2, [r0]
			mov r3, r0

enque_loop:		cmp r2, #0
				bgt link_mov

				bl get_node
				str r0, [r3]
				str r1, [r0,#4]
				b enque_fin
								
link_mov:		ldr r0, [r2]
				b	enque_loop

enque_fin:	sub sp, r11, #4
			pop {r11,pc}


print:		push {r11,lr}
			add r11, sp, #4
			sub sp, sp, #4

			str r1, [r11, #-8]
			mov r2, r1

print_loop:	cmp	r2, #0
			beq	print_fin

			str r2,[r11,#-8]
			ldr r0, =fmt
			ldr r1, [r2,#4]
			bl printf

			ldr r2, [r11, #-8]
			ldr r2, [r2]
			b print_loop

print_fin:	sub sp, r11,#4
			pop {r11,pc}



main:		push {r11,lr}
			add	r11, sp, #4
			sub sp, sp, #8

			mov r0, #0

			str	r0, [r11,#-12]
			sub r0, r11, #12

			mov r2, #1
			str r2, [r11,#-8]

m_loop:		cmp r2, #4
			bgt m_fin

			mov r1, r2
			mov r3, #10
			mul r1, r3

			bl enqueue

			ldr r2, [r11,#-8]
			add r2, r2, #1
			str r2, [r11,#-8]

			b	m_loop

m_fin:		ldr r1, [r11,#-12]
			bl print

			sub sp, r11, #4
			pop {r11,pc}
