;-------------------------------------------------------------------------------
; user gio by kimdonghyeok
;	2019-05-03
;-------------------------------------------------------------------------------
    	.text
    	.arm

gio_a		.word 0xFFF7BC34
gio_reg	.word 0xFFF7BC00
gio_b		.word 0xFFF7BC54
;------------------------------------------------------------------------------
; user gio init
	.global _gio_init_
	.asmfunc

_gio_init_
		ldr r4, gio_reg
		mov r0, #0x1
		str r0, [r4] 			;gioREG->GCR0   = 1
		mov r0, #0xFF
		str r0, [r4,#0x14]	;gioREG->ENACLR = 0xFF
		str r0, [r4,#0x8]		;gioREG->LVLCLR = 0xFF

		ldr r4, gio_b
		mov r0, #0x0040
		str r0, [r4]			;gioPORTB->DIR = 0x0040
		str r0, [r4, #0x1c]  ;gioPORTB->PSL = 0x0040
		mov r0, #0
		bx lr
	.endasmfunc
;-------------------------------------------------------------------------------
;	user gio on
    .global _gio_A5_on_
    .asmfunc

_gio_A5_on_
 		ldr r4, gio_b
 		mov r0, #0x0040
 		str r0, [r4,#0x000C]
 		mov r0, #0
 		bx lr
 	.endasmfunc
;-------------------------------------------------------------------------------
;	user gio off

    .global _gio_A5_off_
    .asmfunc

_gio_A5_off_
 		ldr r4, gio_b
 		mov r0, #0x0040
 		str r0, [r4,#0x0010]
 		mov r0, #0
 		bx lr
 	.endasmfunc
 ;-------------------------------------------------------------------------------
