#include "reg.h"

// forward declarations needed for handshake
void uart_send_string(const char* msg);

// functions for register checks, bits of the mmr help tell
// us the status of what is inside of the register

inline void delay(){
	for(uint32_t i = 0; i < 50000; i++){;}
}

inline void transmit_reg_empty_check(){
	while(!(*USART2_SR & (1 << 7)));
}

inline void read_reg_empty_check(){
	while(!(*USART2_SR & (1 << 5)))
	
}

inline void flash_bsy_wait(){
	while(*FLASH_SR & (1 << 16));
}
