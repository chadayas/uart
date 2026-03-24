#include "reg.h"

inline void clear_uart_err_flags(){
	volatile uint32_t sr = *USART2_SR;
	volatile uint32_t dr = *USART2_DR;
	
}


// forward declarations needed for handshake
void uart_send_string(const char* msg);

// functions for register checks, bits of the mmr help tell
// us the status of what is inside of the register
// the while loop implements a wait
inline void transmit_complete_wait(){
	while(!(*USART2_SR & (1 << 6)));
}

inline void delay(){
	for(uint32_t i = 0; i < 10000; i++){;}
}

inline void transmit_reg_empty_check(){
	// bit 7 TXE bit check if data is transferred to shift register	
	while(!(*USART2_SR & (1 << 7)));
}

inline void read_reg_empty_check(){
	// bit 5 RXNE bit check if recieved data is ready to be read	
	while(!(*USART2_SR & (1 << 5)));
	
}

inline void flash_bsy_wait(){
	// bit 16 wait for current flash memory operation to finish	
	while(*FLASH_SR & (1 << 16));
}


