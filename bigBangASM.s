.include "xc.inc"
.text                       ;BP (put the following data in ROM(program memory))
 ; This is a library, thus it can *not* contain a _main function: the C file will
 ; define main().  However, we ; we will need a .global statement to make available ASM functions to C code.
 ; All functions utilized outside of this file will need to have a leading
 ; underscore (_) and be included in a comment delimited list below.
.global _write_1, _write_0, _oneMilliSec
 
    
;1 code total: 62.5 ns * 20 cycles = 1.25 us 
;T1H: 62.5 ns * 12 cycles = 75.0 us 
;20 cycles - 12 high 8 low
_write_1:
    ;call 2
    bset LATB, #7	    ;high 1
    repeat #9	    ;high 1
    nop		    ;high 10
    bclr LATB, #7	    ;low 1
    nop		    ;low 1
    nop		    ;low 1
    return	    ;low 3 + 2 for call

;0 code total: 62.5 ns * 20 cycles = 1.25 us
;T0H: 62.5 ns * 6 cycles = .375 us    
_write_0:
    ;call 2
    bset LATB, #7	    ;high 1
    repeat #3	    ;1
    nop		    ;4
    bclr LATB, #7	    ;low 1
    repeat #6	    ;low 1
    nop		    ;low 7
    return	    ;low 3 + 2 for call
    
_oneMilliSec:
		    ;if needed reduce 2 cycle on repeat for bra
		    ;2 cycles call
    repeat #15993   ;1 cycle
    nop		    ;15996 cycle
    return	    ;3 cycle
    .end