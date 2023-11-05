;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
LAB1        mov.w   #01, &0200h     ; Set memory location 0x0200 to the value 1
            mov.w   #02, &0202h     ; Set 0x0202 to the value 2
            mov.w   #03, &0204h     ; Set 0x0204 to 3

LINEA       clr     R7              ; Clear registers R7-R10
            clr     R8
            clr     R9
            clr     R10

LINEB       mov.w   &0200h, R7      ; Copy word from 0x0200 to R7
            mov.w   &0202h, R8      ; Copy word from 0x0202 to R8
            mov.w   &0204h, R9      ; Copy word from 0x0204 to R9

LINEC       mov.b   R7, R10         ; Start accumulation in R10 w/R7
            add.b   R8, R10         ; Add R8 to R10
            add.b   R9, R10         ; Add R9 to R10
            mov.b   R10, &0206h     ; Store the sum in 0x0206

Mainloop    jmp     Mainloop        ; Infinite loop (halt)

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
