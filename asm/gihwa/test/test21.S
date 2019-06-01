		.data

fmt:	.asciz "sum = %10f\n"


 
		.text
		.globl main

main:	push {fp, lr}
		add fp, sp, #4
		sub sp, sp, #32	


fcons:	.float 0.0001
		.float 2
		.float 1
		.float 0
		vldr.32 s0, fcons
		vstr.32 s0, [fp, #-12]
				
		vldr.32 s1, fcons +4
		vstr.32 s1, [fp, #-16]

		vldr.32 s2, fcons
		vstr.32 s2, [fp, #-20]

		vdiv.f32 s2, s2, s0
	
		vldr.32 s3, fcons
		vstr.32 s3, [fp, #-24]
		vmov.f32 s3, #0
loop:	mov r2, #0
		vadd.f32 s3,s3,s0
		vadd.f32 s4,s3,s2
		vmul.f32 s4,s4
		vcvt.f64.f32 d16,s4
		vmov r1, r2, d16
		ldr r0, =fmt
		bl printf		
/*
		vadd.f32 s2, s0, s1
		vcvt.f64.f32 d16, s2
		vmov r1, r2, d16
		ldr r0, =fmt
		bl printf
*/
	
		mov r0, #0
		sub sp, fp, #4
		pop {fp, pc}

