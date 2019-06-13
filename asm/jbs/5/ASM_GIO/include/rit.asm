	.text
	.arm

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
