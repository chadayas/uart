.syntax unified
.cpu cortex-m4
.thumb


.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler
.org 0x3C  /*replace the .repr with .org for placing our handlers where we need them*/
.word SysTick_handler
.org 0x98
.word Usart2IQR_handler

.text
.global Reset_Handler
.global Usart2IQR_handler
.global SYSTick_handler

.extern Usart2IQR_handler
.extern SYSTick_handler

.type Reset_Handler, %function
.type Usart2IQR_handler, %function
.type SysTick_handler, %function

Reset_Handler:
    ldr sp, =_estack
    
    bl main
1:  b 1b
.size Reset_Handler, .-Reset_Handler


