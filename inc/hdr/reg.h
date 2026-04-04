#include<cstdint>

// definitions of the used registers for bare metal access
// base defined as integers, we then reinterpret them into 
// pointers to represent a memory addr


constexpr auto USART2_BASE {0x40004400};
constexpr auto RCC_BASE    {0x40023800};
constexpr auto GPIOA_BASE  {0x40020000};
constexpr auto FLASH_BASE  {0x40023C00};

auto const AIRCR {reinterpret_cast<volatile uint32_t*>(0xE000ED0C)};

auto const GPIOA_MODER {reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x00)}; 
auto const GPIOA_AFRL {reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x20)};

auto const RCC_AHB1ENR {reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x30)};
auto const RCC_APB1ENR {reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x40)};

auto const USART2_SR {reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x00)};
auto const USART2_DR {reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x04)};
auto const USART2_BRR {reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x08)};
auto const USART2_CR1 {reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x0C)};
auto const USART2_CR2 {reinterpret_cast<volatile uint32_t*>(USART2_BASE + 0x10)};

auto const FLASH_KEYR {reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x04)};
auto const FLASH_CR {reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x10)};
auto const FLASH_ACR {reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x00)};
auto const FLASH_SR {reinterpret_cast<volatile uint32_t*>(FLASH_BASE + 0x0C)};

