#include<cstdint>
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#include "../inc/hdr/flash.h"

int main()
{
	open_USART_config();
	start_transmission();
	flash_init();
	start_recieve();
	app_jump();
}
