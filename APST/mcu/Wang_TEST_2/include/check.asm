	.text
	.arm

	.global _input_MDIO_
	.asmfunc

_input_MDIO_
	ldr r1, MDIO_USER
	mov r0, #2
	mov r12, #1
	lsl r0, r0, #0x15
	orr r12, r0, r12, lsl #16
	orr r12, r12, #0x2000
	orr r12, r12, #0x20000000
	str	r12, [r1, #0x80]
	bx	lr

MDIO_USER	.word	0xFCF78900

	.endasmfunc
