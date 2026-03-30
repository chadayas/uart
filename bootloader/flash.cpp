#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"
#define REG_KEY 0x05FA

void flash_init(){
	// values must be programmed into the 
	// KEYR register to unlock the CR register
	const uint32_t KEY1 = 0x45670123; 
	const uint32_t KEY2 = 0xCDEF89AB; 
	
	*FLASH_KEYR = KEY1;
	*FLASH_KEYR = KEY2;
	
	// setting PSIZE for our boards voltage range (3.3V)
	// choosing bytes for PSIZE (8-bit)
	*FLASH_CR &= ~(3 << 8);

}

void flash_erase(){
	// wait for any ongoing flash mem operations
	flash_bsy_wait();
	// activate sector erase
	*FLASH_CR |= (1 << 1);

	// erase sectors 2-7, set SNB bits [6:3] per sector number in binary
	// sector 2 = 0010
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 4);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// sector 3 = 0011
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 4) | (1 << 3);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// sector 4 = 0100
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 5);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// sector 5 = 0101
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 5) | (1 << 3);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// sector 6 = 0110
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 5) | (1 << 4);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// sector 7 = 0111
	flash_bsy_wait();
	*FLASH_CR &= ~(0xF << 3);
	*FLASH_CR |= (1 << 5) | (1 << 4) | (1 << 3);
	*FLASH_CR |= (1 << 16);
	flash_bsy_wait();

	// clear SER bit
	*FLASH_CR &= ~(1 << 1);
}

void flash_write(uint32_t dest, uint32_t len){
	uint8_t* ptr = reinterpret_cast<uint8_t*>(dest);
	const uint32_t CHUNK = 32;
	uint8_t buf[CHUNK];

	flash_bsy_wait();
	*FLASH_CR |= (1 << 0);

	for(uint32_t offset = 0; offset < len; offset += CHUNK){
		uint32_t n = ((len - offset) < CHUNK) ? (len - offset) : CHUNK;

		// read chunk into SRAM buffer
		for(uint32_t i = 0; i < n; i++){
			read_reg_empty_check();
			buf[i] = (uint8_t)*USART2_DR;
		}

		// write SRAM buffer to flash
		for(uint32_t i = 0; i < n; i++){
			flash_bsy_wait();
			*ptr++ = buf[i];
			flash_bsy_wait();
		}

		// ACK chunk done, host sends next
		uart_send_byte(0x79);
	}

	*FLASH_CR &= ~(1 << 0);
	uart_send_string("MCU: write complete\n");
}

void app_jump(){
	uint32_t app_pc = *reinterpret_cast<volatile uint32_t*>(0x08008004);
	uint32_t app_sp = *reinterpret_cast<volatile uint32_t*>(0x08008000);
	
	__asm volatile(
			"mov sp, %0\n"
			"bx %1\n"
			: : "r" (app_sp), "r" (app_pc)
		      );
}



// function to implement a physical press of the black RESET
// button on stm32 programmatically
void hardware_reset(){
   // start function when python sends the byte 
   uint8_t reset_byte = *USART2_DR;
   // write the reg key in the upper half of the AIRCR 
   // register, then actiavte SYSRESETREQ
   while(1){ 
      if (reset_byte == 0x80){
         *AIRCR |= (REG_KEY << 16); 
         *AIRCR |= (1 << 2);
         break; 
      }
   }

}

