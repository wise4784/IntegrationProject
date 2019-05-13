		.data
fmt:	.asciz "res float = %f\n"
		
		.text
		.globl main

main:	push {fp, lr}
		add fp, sp, #4
		sub sp, sp, #32

fcons:	.float 0.37373
		.float 0.33773
		vldr.32 s0, fcons
		vstr.32 s0, [fp, #-12]
		vldr.32 s1, fcons +4
		vstr.32 s1, [fp, #-16]
		vadd.f32 s2, s0, s1
		vcvt.f64.f32 d16, s2
		vmov r2, r3, d16
		ldr r0, =fmt
		bl printf
		mov r0, #0
		sub sp, sp, #4
		pop {fp, pc}
