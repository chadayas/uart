#include "../inc/hdr/flash.h"
#include "../inc/hdr/checks.h"
#include "../inc/hdr/uart.h"

volatile uint32_t ticks{};

extern "C" void Usart2IQR_handler(){
     if ((uint8_t )usart2()->DATA_REG == 0x80) hardware_reset();  
}
 
extern "C" void SysTick_handler(){
   ticks++;
}

void init_systick(){
   using namespace SYSTICK; 
   systick()->CTRL_REG |= ( Set::TIMER | Set::TICKINT | Set::CLKSRC );
   systick()->RELOAD_REG |= RVR; 
   systick()->CURRVAL_REG = 0U; 
}

void systick_delay(uint32_t delay_ticks){
   ticks = 0;
   while(ticks < delay_ticks); 
}

void uart_send_string(const char* msg){
	while(*msg){
		transmit_reg_empty_check();
		usart2()->DATA_REG = *msg++;
	}
	transmit_complete_wait();
}

void uart_send_byte(uint8_t byte){
	transmit_reg_empty_check();
	usart2()->DATA_REG = byte;
	transmit_complete_wait();
}

void open_USART_config(){
	rcc()->AHB1ENR_REG |= RCC::Enable::GPIOA;
	rcc()->APB1ENR_REG |= RCC::Enable::USART2;

	gpioa()->MODER_REG |= GPIOA::Set::PA2_AF_MODE;
	gpioa()->MODER_REG |= GPIOA::Set::PA3_AF_MODE;

	gpioa()->AFRL_REG |= GPIOA::Set::USART2_TX;
	gpioa()->AFRL_REG |= GPIOA::Set::USART2_RX;

	usart2()->BAUDRATE_REG = USART2::Set::BAUD_RATE; // baud rate 115200 at 16MHz HSI

	usart2()->CTRL_REG |= USART2::Enable::USART;
}

void start_transmission(){
	using namespace USART2::Status;

	usart2()->CTRL_REG |= USART2::Enable::TX_PIN;
	usart2()->CTRL_REG |= USART2::Enable::RX_PIN;
   
   init_systick(); 
	
   // send 0x7F until host responds with 0x79
	uint32_t attempts = 0;
	while(1){
		uart_send_byte(0x7F);
		systick_delay(100);
		if(RDR_NOT_EMPTY()){
			if((uint8_t)usart2()->DATA_REG == 0x79) break;
		}
		if(attempts++ > 1000) app_jump();
	}
	start_recieve();
}

void start_recieve(){
	// receive firmware size (4 bytes, little endian)
	uint32_t len = 0;
   for(int i{}; i < 4; i++){
		read_reg_empty_check();
		len |= (usart2()->DATA_REG << (i * 8));
	}

	uart_send_string("MCU: erasing flash\n");
	flash_erase();

	// signal host erase is done, ready for binary
	uart_send_byte(0x79);

	flash_write(0x08008000, len);
}


