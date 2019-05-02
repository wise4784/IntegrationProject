		.macro SHIFT a, b
    	.if \b <0
    	mov \a, \a, asr #-\b
    	.else
    	mov \a, \a, asr #\b
    	.endif
    	.endm

	    .data

fmt:    .asciz "Hello, %d, %d\n"

        .text
        .globl   main

main:   stmfd sp!, {lr}
		mov r1, #32
		mov r2, r1
        SHIFT r1, 2
		SHIFT r2, -1
        ldr r0, =fmt
        bl printf
        mov r0, #0
        ldmfd sp!, {lr}
        mov pc, lr

