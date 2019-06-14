 		.text
 		.arm

 		.global	_gio_a5_on_
 		.asmfunc

_gio_a5_on_
	; for gioREG Setting
	ldr	r4, gio_reg ; ldr: 포인터 참조 연산과 같은 일을 했다.
 	mov r6, #0x01
 	str r6, [r4]
 	mov r6, #0xFF
 	str r6, [r4, #0x14]
 	str r6, [r4, #0x08]

 	; for gioPORTA Setting
 	ldr r5, gio_a
 	mov r6, #0x20
 	str r6, [r5]
 	str r6, [r5, #0x1C]
 	str r6, [r5, #0x08]
 	bx	lr

gio_reg	.word	0xFFF7BC00
gio_a	.word	0xFFF7BC34
	.endasmfunc

	.global _disable_all_
	.asmfunc

_disable_all_
	cpsid if

	;cps #16
	;mrs r1, cpsr
	;orr r1, r1, #0xc0
	;msr spsr_sxcf, r1

	;cps #17
	;mrs r1, cpsr
	;orr r1, r1, #0xc0
	;msr spsr_sxcf, r1

	;cps #19
	;mrs r1, cpsr
	;orr r1, r1, #0xc0
	;msr spsr_sxcf, r1

	;cps #0x1f
	;mrs r1, cpsr
	;orr r1, r1, #0xc0
	;msr spsr_sxcf, r1
	bx lr
	.endasmfunc
fiq_reg	.word	0x6000039F


	.global _print_cpsr_
    .asmfunc

_print_cpsr_
   mrs r0, cpsr
   bx lr
   .endasmfunc


    .global _print_spsr_
    .asmfunc

_print_spsr_
   mrs r0, spsr
   bx lr
   .endasmfunc

