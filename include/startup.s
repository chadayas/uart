.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler

.text
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_estack
    mov sp, r0

    bl main
1:  b 1b
.size Reset_Handler, .-Reset_Handler


