#include<cstdint>

// definitions of the used registers for bare metal access
// base defined as integers, we then reinterpret them into 
// pointers to represent a memory addr


#define USART2_BASE 0x40004400
#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000
#define FLASH_BASE  0x40023C00

#define AIRCR reinterpret_cast<volatile uint32_t*>(0xE000ED0C)

#define GPIOA_MODER reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x00)
#define GPIOA_AFRL  reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x20)

#define RCC_AHB1ENR reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x30)
#define RCC_APB1ENR reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x40)

#define USART2_SR  reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x00)
#define USART2_DR  reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x04)
#define USART2_BRR reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x08)
#define USART2_CR1 reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x0C)
#define USART2_CR2 reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x10)

#define FLASH_KEYR reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x04)
#define FLASH_CR reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x10)
#define FLASH_ACR reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x00)
#define FLASH_SR reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x0C)

