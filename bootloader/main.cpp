#include<cstdint>
#include "../inc/hdr/reg.h"



inline void transmit_reg_empty_check(){
	while(!(*USART2_SR & (1 << 7)));
}

void uart_send_string(const char* msg){
	while(*msg){
		transmit_reg_empty_check();
		*USART2_DR = *msg++;
	}
	// wait for last bit to fully transmit
	while(!(*USART2_SR & (1 << 6)));
}

inline void delay(){
	for(uint32_t i = 0; i < 50000; i++){;}
}



inline void read_reg_empty_check(){
	while(!(*USART2_SR & (1 << 5))){
		uart_send_string("handshake\n");
		delay();	
	}
}
inline void flash_bsy_wait(){
	while(*FLASH_SR & (1 << 16));
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

	// wait untill hardware sets TXE bit to 1 indicating 
	// DR is empty, then fill with our char x
	// loop through char array for send	
	
	const char text[] = "TESTING\n";	
	uart_send_string(text);	

}



void flash_init(){
	// values must be programmed into the 
	// KEYR register to unlock the CR register
	const uint32_t KEY1 = 0x45670123; 
	const uint32_t KEY2 = 0xCDEF89AB; 
	
	*FLASH_KEYR = KEY1;
	*FLASH_KEYR = KEY2;
	
	if(*FLASH_CR & (1 << 31)){
		uart_send_string("FLASH INIT: FLASH_CR is locked\n");
	} else{
		uart_send_string("FLASH INIT: FLASH_CR is unlocked\n");
	}
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

	uart_send_string("BOOT: Erasing flash from 0x08008000 to 0x0807FFFF\n");
	flash_erase();
	uart_send_string("BOOT: flash erased\n");

	flash_bsy_wait();
	// enable flash programming
	*FLASH_CR |= (1 << 0);

	for(uint32_t i = 0; i < len; i++){
		uart_send_string("WRITE: flash write occuring...\n");	
		flash_bsy_wait();
		// receive byte over UART
		read_reg_empty_check();
		uint8_t byte = *USART2_DR;
		// write byte to flash
		*ptr++ = byte;
		flash_bsy_wait();
	}

	// clear PG bit
	*FLASH_CR &= ~(1 << 0);
	uart_send_string("BOOT: flash write complete\n");
	
}

void start_recieve(){
	// RE bit enabled, search for start bit
	*USART2_CR1 |= (1 << 2);

	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
	for(int i = 0; i < 4; i++){
		read_reg_empty_check();
		len |= (*USART2_DR << (i * 8));
	}
	
	if(len == 0){
		uart_send_string("RECIEVE: len is zero, flash write corrupted\n");
	}else if(len > 0){
		uart_send_string("RECIEVE: len is greater then zero\n"); 
	}else if(len < 10){
		uart_send_string("RECIEVE: len is less then 10, send not complete\n"); 
	}else if(len > 100){
		uart_send_string("RECIEVE: len is greater then 100, send may be complete\n"); 
	}
	
	// receive firmware and write to flash
	flash_write(0x08008000, len);
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

int main()
{
	open_USART_config();
	start_transmission();
	flash_init();
	start_recieve();
	app_jump();
}
