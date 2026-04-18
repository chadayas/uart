#include<cstdint>
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#include "../inc/hdr/flash.h"

// function to implement a physical press of the black RESET
// button on stm32 programmatically

__attribute__((section(".isr_vector"))) 
void Usart2_IQR(){
     if ((uint8_t )usart2()->DATA_REG == 0x80) hardware_reset();  
}

int main()

{
   open_USART_config();
	flash_init();
   start_transmission();
	app_jump();
	
}

