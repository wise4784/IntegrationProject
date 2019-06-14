	.arch armv5t
	.fpu vfpv3-fp16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"float_cc.c"
	.text
	.align	2
	.global	test
	.syntax unified
	.arm
	.type	test, %function
test:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]	@ float
	str	r1, [fp, #-12]	@ float
	vldr.32	s14, [fp, #-8]
	vldr.32	s15, [fp, #-12]
	vadd.f32	s15, s14, s15
	vmov	r3, s15
	mov	r0, r3	@ float
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	test, .-test
	.section	.rodata
	.align	2
.LC0:
	.ascii	"c = %f\012\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	ldr	r3, .L5
	str	r3, [fp, #-16]	@ float
	ldr	r3, .L5+4
	str	r3, [fp, #-12]	@ float
	ldr	r1, [fp, #-12]	@ float
	ldr	r0, [fp, #-16]	@ float
	bl	test
	str	r0, [fp, #-8]	@ float
	vldr.32	s15, [fp, #-8]
	vcvt.f64.f32	d16, s15
	vmov	r2, r3, d16
	ldr	r0, .L5+8
	bl	printf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
.L6:
	.align	2
.L5:
	.word	1051521072
	.word	1052727690
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
