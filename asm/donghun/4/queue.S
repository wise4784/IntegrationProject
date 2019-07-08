	    .data
queue:	    stmfd sp!, {lr}
	    .word	@ link
	    .word	@ data
	    ldmfd sp!, {lr}
	    mov pc, lr

	    .text
	    .globl main

enqueue:    stmfd sp!, {lr}
	    ldmfd sp!, {lr}
	    mov pc, lr

main:	    stmfd sp!, {lr}
	    bl malloc
	    mov r0, #0
	    ldmfd sp!, {lr}
	    mov pc, lr
