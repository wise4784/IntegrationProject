	.arch armv5t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"queue.c"
	.text
	.align	2
	.global	get_node
	.syntax unified
	.arm
	.type	get_node, %function
get_node:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	mov	r0, #8
	bl	malloc
	mov	r3, r0
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
	.size	get_node, .-get_node
	.align	2
	.global	enqueue
	.syntax unified
	.arm
	.type	enqueue, %function
enqueue:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	cmp	r3, #0
	bne	.L4
	bl	get_node
	mov	r2, r0
	ldr	r3, [fp, #-8]
	str	r2, [r3]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	ldr	r2, [fp, #-12]
	str	r2, [r3, #4]
	b	.L3
.L4:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	ldr	r1, [fp, #-12]
	mov	r0, r3
	bl	enqueue
.L3:
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
	.size	enqueue, .-enqueue
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	ldr	r3, .L9
	ldr	r3, [r3]
	str	r3, [fp, #-8]
	mov	r3, #0
	str	r3, [fp, #-12]
	sub	r3, fp, #12
	mov	r1, #10
	mov	r0, r3
	bl	enqueue
	mov	r3, #0
	mov	r0, r3
	ldr	r3, .L9
	ldr	r2, [fp, #-8]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L8
	bl	__stack_chk_fail
.L8:
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
.L10:
	.align	2
.L9:
	.word	__stack_chk_guard
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
