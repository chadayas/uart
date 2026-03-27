#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"

void uart_send_string(const char* msg){
	while(*msg){
		transmit_reg_empty_check();
		*USART2_DR = *msg++;
	}
	transmit_complete_wait();
}

void uart_send_byte(uint8_t byte){
	transmit_reg_empty_check();
	*USART2_DR = byte;
	transmit_complete_wait();
}

void open_USART_config(){
	// enable clock for GPIOA and USART2
	*RCC_AHB1ENR |= (1 << 0);
	*RCC_APB1ENR |= (1 << 17);

	// PA2 and PA3 to alternate function mode
	*GPIOA_MODER |= (1 << 5);
	*GPIOA_MODER |= (1 << 7);

	// AF7 for USART2_TX (PA2)
	*GPIOA_AFRL |= (1 << 10);
	*GPIOA_AFRL |= (1 << 9);
	*GPIOA_AFRL |= (1 << 8);

	// AF7 for USART2_RX (PA3)
	*GPIOA_AFRL |= (1 << 14);
	*GPIOA_AFRL |= (1 << 13);
	*GPIOA_AFRL |= (1 << 12);

	// baud rate 115200 at 16MHz HSI
	*USART2_BRR = (8 << 4) | 11;

	// enable USART
	*USART2_CR1 |= (1 << 13);
}

void start_transmission(){
	// enable TX and RX
	*USART2_CR1 |= (1 << 3);
	*USART2_CR1 |= (1 << 2);
   
   uint32_t attempts = 0;
	// send 0x7F until host responds with 0x79
	while(1){
		uart_send_byte(0x7F);
		delay();
		if(*USART2_SR & (1 << 5)){
			if((uint8_t)*USART2_DR == 0x79) break;
		}
      if (attempts++ > 1000) app_jump();	
   }
	start_recieve();
}

void start_recieve(){
	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
	for(int i = 0; i < 4; i++){
		read_reg_empty_check();
		len |= (*USART2_DR << (i * 8));
	}

	uart_send_string("MCU: erasing flash\n");
	flash_erase();
	
   // signal host erase is done, ready for binary
	uart_send_byte(0x79);

	flash_write(0x08008000, len);
}
