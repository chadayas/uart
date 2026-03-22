#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"


void open_USART_config(){
	// enable clock for bus for GPIOA and USART2	
	*RCC_AHB1ENR |= (1 << 0);
       	*RCC_APB1ENR |= (1 << 17);
	// change mode to alternate function for PA2	
	*GPIOA_MODER |= (1 << 5);
       // change mode to alternate function for PA3	
	*GPIOA_MODER |= (1 << 7);
       

	// alternate function selection for USART2_TX  
	*GPIOA_AFRL |= (1 << 10); 
	*GPIOA_AFRL |= (1 << 9); 
	*GPIOA_AFRL |= (1 << 8);

	// alternate function selection for USART2_RX  
	*GPIOA_AFRL |= (1 << 14); 
	*GPIOA_AFRL |= (1 << 13); 
	*GPIOA_AFRL |= (1 << 12);

	// setting baud rate at 115200
	*USART2_BRR = (8 << 4) | 11;

	// enable USART
	*USART2_CR1 |= (1 << 13);
};

void start_transmission(){

	// enable the transmitter
	*USART2_CR1 |= (1 << 3); 

	// wait untill hardware sets TXE bit to 1 indicating 
	// DR is empty, then fill with our char x
	// loop through char array for send	
	
	const char text[] = "TESTING\n";	
	uart_send_string(text);	

}

void start_recieve(){
	// RE bit enabled, search for start bit
	*USART2_CR1 |= (1 << 2);

	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
	for(int i = 0; i < 4; i++){
		read_reg_empty_check();
		len |= (*USART2_DR << (i * 8));
	}
	
	if(len == 0){
		uart_send_string("RECIEVE: len is zero, flash write corrupted\n");
	}else if(len > 0){
		uart_send_string("RECIEVE: len is greater then zero\n"); 
	}else if(len < 10){
		uart_send_string("RECIEVE: len is less then 10, send not complete\n"); 
	}else if(len > 100){
		uart_send_string("RECIEVE: len is greater then 100, send may be complete\n"); 
	}
	
	// receive firmware and write to flash
	flash_write(0x08008000, len);
}	

