;-------------------------------------------------------------------------------
; user disable interrupt by kimdonghyeok
;	2019-05-03
;-------------------------------------------------------------------------------
    	.text
    	.arm

;-------------------------------------------------------------------------------
;	user disable interrupt
    .global _disable_ALL_interrupt_
    .asmfunc

_disable_ALL_interrupt_
;        cpsid i //i-irq f-fiq
       cpsid if ;i-irq f-fiq
;		cps	#17
;		mrs	r1, spsr
;		orr	r1,	r1,	#0xc0
;		msr	spsr_cxsf,	r1
       bx    lr
 	.endasmfunc
;-------------------------------------------------------------------------------
