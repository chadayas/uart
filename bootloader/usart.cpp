#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"

void uart_send_hs(int bytes){
	// used to send confirmation bytes to host	
	transmit_reg_empty_check();
	*USART2_DR = bytes;		
}

void uart_send_string(const char* msg){
	while(*msg){
		transmit_reg_empty_check();
		*USART2_DR = *msg++;
	}
}

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
	
	// poke host to let them know we are ready
	int rdy = 0x7f;	
	while(!(*USART2_SR & (1 << 7))){
		uart_send_hs(ACK);

	}	
}

void start_recieve(){
	// TODO: implement a wait loop until we recieve
	// the bytes from host to indicate it is ready 
	// for more data transmission
	
	// RE bit enabled, search for start bit
	*USART2_CR1 |= (1 << 2);
	
	read_reg_empty_check();
	if((char)*USART_DR == "host_rdy"){

	}

	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
	for(int i = 0; i < 4; i++){
		read_reg_empty_check();
		len |= (*USART2_DR << (i * 8));
	}
	

	// receive firmware and write to flash
	flash_write(0x08008000, len);
}	

