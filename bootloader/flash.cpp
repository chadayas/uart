#include "../inc/hdr/flash.h"
#include "../inc/hdr/reg.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"


void flash_init(){
	// values must be programmed into the 
	// KEYR register to unlock the CR register
   using namespace FLASH; 
   flash()->KEY_REG = KEY1;
   flash()->KEY_REG = KEY2;
   
	// setting PSIZE for our boards voltage range (3.3V)
	// choosing bytes for PSIZE (8-bit)
   flash()->CTRL_REG &= Set::PSIZE; 
}

void flash_erase(){
   using namespace FLASH;
   // wait for any ongoing flash mem operations
	flash_bsy_wait();

   flash()->CTRL_REG |= Enable::SECTOR_ERASE;

	// erase sectors 2-7, set SNB bits [6:3] per sector number in binary
	// sector 2 = 0010
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_2;
   flash()->CTRL_REG |= Enable::ERASE;
   flash_bsy_wait();

	// sector 3 = 0011
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_3;
   flash()->CTRL_REG |= Enable::ERASE;
	flash_bsy_wait();

	// sector 4 = 0100
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_4;
   flash()->CTRL_REG |= Enable::ERASE;
	flash_bsy_wait();

	// sector 5 = 0101
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_5;
   flash()->CTRL_REG |= Enable::ERASE;
	flash_bsy_wait();

   // sector 6 = 0110
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_6;
   flash()->CTRL_REG |= Enable::ERASE;
	flash_bsy_wait();

	// sector 7 = 0111
	flash_bsy_wait();
   flash()->CTRL_REG &= Clear::SNB_BITS;
   flash()->CTRL_REG |= Set::sector_7;
   flash()->CTRL_REG |= Enable::ERASE;
	flash_bsy_wait();

	// clear SER bit
   flash()->CTRL_REG &= Clear::SER_BIT;
}

void flash_write(uint32_t dest, uint32_t len){
   using namespace FLASH;
   uint8_t* ptr = reinterpret_cast<uint8_t*>(dest);
	const uint32_t CHUNK = 32;
	uint8_t buf[CHUNK];

   flash_bsy_wait();
   flash()->CTRL_REG |= Enable::PROGRAMMING;

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

   flash()->CTRL_REG &= Clear::PROGRAMMING; 
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


