#include<cstdint>
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#include "../inc/hdr/flash.h"


int main()

{
   open_USART_config();
	flash_init();
   start_transmission();
	app_jump();
	
}

