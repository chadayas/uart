.syntax unified
.cpu cortex-m4
.thumb


.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler
.rept 52
.word 0 
.endr
.word Usart2IQR_handler

.text
.global Reset_Handler
.global Usart2IQR_handler

.extern Usart2IQR_handler

.type Reset_Handler, %function
.type Usart2IQR_handler, %function


Reset_Handler:
    ldr r0, =_estack
    mov sp, r0

    bl main
1:  b 1b
.size Reset_Handler, .-Reset_Handler


