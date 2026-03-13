/*
 transmitter procedure

1. Enable the USART by writing the UE bit in USART_CR1 register to 1.

2. Program the M bit in USART_CR1 to define the word length.

3. Program the number of stop bits in USART_CR2.

4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
place. Configure the DMA register as explained in multibuffer communication.

5. Select the desired baud rate using the USART_BRR register.

6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.

7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
for each data to be transmitted in case of single buffer.

8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
that the transmission of the last frame is complete. This is required for instance when
the USART is disabled or enters the Halt mode to avoid corrupting the last
transmission.

   receiver procedure

1. Enable the USART by writing the UE bit in USART_CR1 register to 1.

2. Program the M bit in USART_CR1 to define the word length.

3. Program the number of stop bits in USART_CR2.

4. Select DMA enable (DMAR) in USART_CR3 if multibuffer communication is to take
place. Configure the DMA register as explained in multibuffer communication. STEP 3

5. Select the desired baud rate using the baud rate register USART_BRR

6. Set the RE bit USART_CR1. This enables the receiver that begins searching for a start
bit.

*/

// PA2 controls the UART_TX

#include<cstdint>

#define USART2 0x40004400 
#define RCC 0x40023800
#define GPIOA_BASE 0x40020000
#define GPIOA_MODER reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x00)

#define RCC_AHB1ENR reinterpret_cast<volatile uint32_t*>(RCC + 0x30)
#define RCC_APB1ENR reinterpret_cast<volatile uint32_t*>(RCC + 0x30)

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

	// enable USART	
	*USART2_CR1 |= (1 << 13);

	// setting baud rate at 115200	
	*USART2_BRR = (8 << 4) | 11; 
};

void start_transmission(){
	
	// enable the transmitter
	*USART2_CR1 |= (1 << 3); 

	// wait untill hardware sets TXE bit to 1 indicating 
	// DR is empty, then fill with our char T
	while(!(*USART2_SR & (1 << 7))); 
	*USART2_DR = 'T';


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
