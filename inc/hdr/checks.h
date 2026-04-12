#include "reg.h"
#define REG_KEY 0x05FAU

// sequentially reading of status register 
// and data register to clear any uart error flags
// (ORE, FE, and etc)
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
	for(uint32_t i = 0; i < 500000; i++){;}
}

inline void transmit_reg_empty_check(){
	// bit 7 TXE bit check if data is transferred to shift register	
	while(!(*USART2_SR & (1 << 7)));
}

inline void read_reg_empty_check(){
	clear_uart_err_flags();
	// bit 5 RXNE bit check if recieved data is ready to be read	
	while(!(*USART2_SR & (1 << 5)));
	
}

inline void flash_bsy_wait(){
	// bit 16 wait for current flash memory operation to finish	
	while(*FLASH_SR & (1 << 16));
}


 // function to implement a physical press of the black RESET
 // button on stm32 programmatically
 
inline void hardware_reset(){
    asm volatile("dsb 0xf":::"memory"); // data sync buffer, post a wait until key is written
 
    constexpr uint32_t mask = (REG_KEY << 16U) | // write key onto upper part of register
                              (7U << 8U) |// leave priority group unchanged
                              (1U << 2U); // set SYSRESETREQ bit
    *AIRCR = mask;
 
    asm volatile("dsb 0xf":::"memory");
    while(1); // spin program so no other code is ran
}
 
