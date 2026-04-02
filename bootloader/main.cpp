#include<cstdint>
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#include "../inc/hdr/flash.h"
#define REG_KEY 0x05FA

// function to implement a physical press of the black RESET
// button on stm32 programmatically
void hardware_reset(){
   *AIRCR |= (REG_KEY << 16); 
   *AIRCR |= (1 << 2);

}
void wait_hw_reset(){
   // start function when python sends the byte 
   // write the reg key in the upper half of the AIRCR 
   // register, then actiavte SYSRESETREQ
   uint32_t attempts = 0; 
   while(1){	  
      if(*USART2_SR & (1 << 5)){
         read_reg_empty_check(); 
         uint8_t reset_byte = (uint8_t)*USART2_DR;
         if(reset_byte == 0x80){
            *AIRCR |= (REG_KEY << 16); 
            *AIRCR |= (1 << 2);
               }
         }
      if(attempts == 20){
         hardware_reset();  
         break;
      }
      attempts++; 
   }
}

int main()
{
   hardware_reset(); 
   open_USART_config();
	flash_init();
   start_transmission();
	app_jump();
	
}
