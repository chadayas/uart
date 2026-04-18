#include "regreg.h"

// sequentially reading of status register 
// and data register to clear any uart error flags
// (ORE, FE, and etc)
inline void clear_uart_err_flags(){
   volatile uint32_t sr = usart2()->STATUS_REG;
   volatile uint32_t dr = usart2()->DATA_REG;
   // use variables so we dont get warnings 
   (void *)(sr); (void *)(dr);
}


// forward declarations needed for handshake
void uart_send_string(const char* msg);

// functions for register checks, bits of the mmr help tell
// us the status of what is inside of the register
// the while loop implements a wait

inline void transmit_complete_wait(){
   using namespace USART2::Status;	
   while( !(TRANSMIT_COMPLETE()) );
}

inline void delay(){
	for(uint32_t i {}; i < 500000; i++){;}
}

inline void transmit_reg_empty_check(){
   using namespace USART2::Status;	
   while( !(TDR_IS_EMPTY()) );
}

inline void read_reg_empty_check(){
   using namespace USART2::Status;	
	while( !(RDR_NOT_EMPTY()) );
	
}

inline void flash_bsy_wait(){
   using namespace FLASH::Status;	
	while( FLASH_BUSY() );
}


 // function to implement a physical press of the black RESET
 // button on stm32 programmatically
 
inline void hardware_reset(){
   using namespace AIRCR;  
    Run::_DSB(); 

    aircr()->BASE = Enable::SYS_RESET;
 
    Run::_DSB(); 
    
    while(1); // spin program so no other code is ran
}

extern "C" void Usart2IQR_handler();
