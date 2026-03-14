
#include<cstdint>

#define USART2 0x40004400 
#define RCC 0x40023800
#define GPIOA_BASE 0x40020000
#define GPIOA_MODER reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x00)

#define RCC_AHB1ENR reinterpret_cast<volatile uint32_t*>(RCC + 0x30)
#define RCC_APB1ENR reinterpret_cast<volatile uint32_t*>(RCC + 0x40)

#define GPIOA_AFRL reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x20)

#define USART2_SR reinterpret_cast<volatile uint32_t*>(USART2 + 0x00)
#define USART2_CR1 reinterpret_cast<volatile uint32_t*>(USART2 + 0x0c)
#define USART2_CR2 reinterpret_cast<volatile uint32_t*>(USART2 + 0x10)
#define USART2_BRR reinterpret_cast<volatile uint32_t*>(USART2 + 0x08)
#define USART2_DR reinterpret_cast<volatile uint32_t*>(USART2 + 0x04)

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
	const char text[] = "\n SLATT SLATT SLATT";	
	for(auto& x : text ){	
		while(!(*USART2_SR & (1 << 7))); 
		*USART2_DR = x;
	}

}

void start_recieve(){
	// RE bit enabled, search for start bit
	*USART2_CR1 |= (1 << 2); 

	while(!(*USART2_SR & (1 << 5)));
	uint8_t buf = *USART2_DR;
}

int main()
{
	open_USART_config();
	start_transmission();
}
