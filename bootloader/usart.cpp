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
	// enable tx and rx
	uint32_t attempts = 0;	
	*USART2_CR1 |= (1 << 3); 
	*USART2_CR1 |= (1 << 2);

	// poke host to let them know we are ready
	while(1){
		uart_send_hs(0x7f);
		delay();
		
		if(*USART2_SR & (1 << 5)){
			int resp = *USART2_DR;
			if(resp == 0x79){ 
			uart_send_string("COMMS: host is ready\n");	
			start_recieve();	
			break;
			}
		}
		if (attempts > 1000){
			uart_send_string("COMMS: unable to recv start byte");
			break;	
		}	
	}	
}

void start_recieve(){
	
	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
	for(int i = 0; i < 4; i++){
		read_reg_empty_check();
		len |= (*USART2_DR << (i * 8));
	}
	if(len) uart_send_string("COMMS: len was recieved");
	// receive firmware and write to flash
	flash_write(0x08008000, len);
}	

