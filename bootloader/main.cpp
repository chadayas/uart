#include<cstdint>
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#include "../inc/hdr/flash.h"

// function to implement a physical press of the black RESET
// button on stm32 programmatically


int main()

{
   open_USART_config();
	flash_init();
   start_transmission();
	app_jump();
	
}

