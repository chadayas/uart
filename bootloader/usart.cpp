#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"

// type prefix: 0x01 = protocol byte, 0x02 = debug string
void uart_send_proto(uint8_t byte){
	transmit_reg_empty_check();
	*USART2_DR = 0x01;
	transmit_reg_empty_check();
	*USART2_DR = byte;
	transmit_complete_wait();
}

void uart_send_string(const char* msg){
	transmit_reg_empty_check();
	*USART2_DR = 0x02;
	while(*msg){
		transmit_reg_empty_check();
		*USART2_DR = *msg++;
	}
	transmit_complete_wait();
}

// sends send_byte repeatedly until expect_byte is received back from host
void wait_for_ack(uint8_t send_byte, uint8_t expect_byte){
	while(1){
		uart_send_proto(send_byte);
		delay();
		if(*USART2_SR & (1 << 5)){
			if((uint8_t)*USART2_DR == expect_byte) break;
		}
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
}

void start_transmission(){
	// enable TX and RX
	*USART2_CR1 |= (1 << 3);
	*USART2_CR1 |= (1 << 2);

	// initial handshake — raw 0x7F, no type prefix
	// host responds with raw 0x79 to confirm it is alive
	while(1){
		transmit_reg_empty_check();
		*USART2_DR = 0x7F;
		transmit_complete_wait();
		delay();
		if(*USART2_SR & (1 << 5)){
			if((uint8_t)*USART2_DR == 0x79) break;
		}
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

	if(len) uart_send_string("BOOT: firmware size received\n");

	// gate 1: length received, about to erase — wait for host to confirm
	wait_for_ack(0x79, 0x79);

	flash_erase();

	// gate 2: erase done — wait for host to confirm before streaming binary
	wait_for_ack(0x79, 0x79);

	flash_write(0x08008000, len);
}
