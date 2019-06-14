;
;   sys_flush_cache.asm
;
;   Created on: 16 Mar 2016
;       Author: stuhssn@gmail.com
;
;   Flushes a defined region from the cache to RAM
;
;
    .text
    .arm

;-------------------------------------------------------------------------------
 ; dcacheCleanRange
 ; void _dcacheCleanRange_(unsigned int startAddress, unsigned int endAddress);
 	.def _dcacheCleanRange_
 	.asmfunc

_dcacheCleanRange_
 		BIC R0, R0, #7 ; data cache line size - 1
dcloop 	MCR P15, #0, R0, C7, C10, #1 ; clean D entry
 		ADD R0, R0, #8 ; data cache line size
 		CMP R0, R1
 		BLO dcloop
 		MCR P15, #0, R0, C7, C10, #4 ; data Synchronization Barrier

        bx    lr

	.endasmfunc
