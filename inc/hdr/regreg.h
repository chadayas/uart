#ifndef GUARD_REG_H
#define GUARD_REG_H

#include<cstddef>

namespace RCC {
   constexpr uint32_t BASE { 0x40023800 };

   struct Register{
      uint32_t NOT_USING1[12];
      volatile uint32_t AHB1ENR_REG; // offset 0x30
      uint32_t NOT_USING2[3];
      volatile uint32_t APB1ENR_REG; // offset 0x40
   };

   namespace Enable{
      constexpr uint32_t GPIOA { (1U << 0U) };
      constexpr uint32_t USART2 { (1U << 17U) };
   }
}

static_assert(offsetof(RCC::Register, AHB1ENR_REG) == 0x30, "AHB1ENR_REG at wrong offset");
static_assert(offsetof(RCC::Register, APB1ENR_REG) == 0x40, "APB1ENR_REG at wrong offset");

namespace GPIOA {
   constexpr uint32_t BASE { 0x40020000 };

   struct Register{
      volatile uint32_t MODER_REG; // offset 0x00
      uint32_t NOT_USING[7];
      volatile uint32_t AFRL_REG; // offset 0x20
   };

   namespace Set{
      constexpr uint32_t PA2_AF_MODE { (1U << 5U) };
      constexpr uint32_t PA3_AF_MODE { (1U << 7U) };

      constexpr uint32_t USART2_TX { (1U << 10U) | (1U << 9U) | (1U << 8U) };
      constexpr uint32_t USART2_RX { (1U << 14U) | (1U << 13U) | (1U << 12U) };
   }
}

static_assert(offsetof(GPIOA::Register, MODER_REG) == 0x00, "MODER_REG at wrong offset");
static_assert(offsetof(GPIOA::Register, AFRL_REG)  == 0x20, "AFRL_REG at wrong offset");

namespace USART2 {
   constexpr uint32_t BASE { 0x40004400 };

   struct Register{
      volatile uint32_t STATUS_REG;   // offset 0x00
      volatile uint32_t DATA_REG;     // offset 0x04
      volatile uint32_t BAUDRATE_REG; // offset 0x08
      volatile uint32_t CTRL_REG;     // offset 0x0C
   };

   namespace Enable{
      constexpr uint32_t USART  { (1U << 13U) };
      constexpr uint32_t TX_PIN { (1U << 3U) };
      constexpr uint32_t RX_PIN { (1U << 2U) };
      constexpr uint32_t RXNE_Intrpt{ (1U << 5U) };
       
   }

   namespace Set{
      constexpr uint32_t BAUD_RATE = (8U << 4U) | 11U; // 460800 at 16MHz HSI
                                                      //  (8u << 4u) | 11u
                                                      //  115200 baudrate
   }

   namespace Status{
      inline uint32_t RDR_NOT_EMPTY() {
        USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
        return reg->STATUS_REG & (1U << 5U);
      }
      inline uint32_t TDR_IS_EMPTY() {
         USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
         return reg->STATUS_REG & (1U << 7U);
      }
      inline uint32_t TRANSMIT_COMPLETE(){
         USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
         return reg->STATUS_REG & (1U << 6U);
      }
   }
}

static_assert(offsetof(USART2::Register, STATUS_REG)   == 0x00, "STATUS_REG at wrong offset");
static_assert(offsetof(USART2::Register, DATA_REG)     == 0x04, "DATA_REG at wrong offset");
static_assert(offsetof(USART2::Register, BAUDRATE_REG) == 0x08, "BAUDRATE_REG at wrong offset");
static_assert(offsetof(USART2::Register, CTRL_REG)     == 0x0C, "CTRL_REG at wrong offset");

namespace FLASH{
   constexpr uint32_t BASE { 0x40023C00 };

   constexpr uint32_t KEY1 = 0x45670123U;
   constexpr uint32_t KEY2 = 0xCDEF89ABU;

   struct Register{
      uint32_t NOT_USING1[1];        // ACR     offset 0x00
      volatile uint32_t KEY_REG;     //         offset 0x04
      uint32_t NOT_USING2[1];        // OPTKEYR offset 0x08
      volatile uint32_t STATUS_REG;  //         offset 0x0C
      volatile uint32_t CTRL_REG;    //         offset 0x10
   };

   namespace Enable{
      constexpr uint32_t SECTOR_ERASE { (1U << 1U) };
      constexpr uint32_t ERASE        { (1U << 16U) };
      constexpr uint32_t PROGRAMMING { (1U << 0U) };
   }
   namespace Clear{
      constexpr uint32_t SNB_BITS { ~(0xFU << 3U) };
      constexpr uint32_t SER_BIT  { ~(1U << 1U) };
      constexpr uint32_t PROGRAMMING { ~(1U << 0U) };
   }
   namespace Set{
      constexpr uint32_t sector_2 { (1U << 4U) };
      constexpr uint32_t sector_3 { (1U << 4U) | (1U << 3U) };
      constexpr uint32_t sector_4 { (1U << 5U) };
      constexpr uint32_t sector_5 { (1U << 5U) | (1U << 3U) };
      constexpr uint32_t sector_6 { (1U << 5U) | (1U << 4U) };
      constexpr uint32_t sector_7 { (1U << 5U) | (1U << 4U) | (1U << 3U) };
      
      constexpr uint32_t PSIZE {~(3U << 8U)}; 
   }
   namespace Status{
      inline uint32_t FLASH_BUSY(){
         FLASH::Register* reg = reinterpret_cast<FLASH::Register*>(FLASH::BASE);
         return reg->STATUS_REG & (1U << 16U);
      }
   }
}

static_assert(offsetof(FLASH::Register, KEY_REG)    == 0x04, "KEY_REG at wrong offset");
static_assert(offsetof(FLASH::Register, STATUS_REG) == 0x0C, "STATUS_REG at wrong offset");
static_assert(offsetof(FLASH::Register, CTRL_REG)   == 0x10, "CTRL_REG at wrong offset");

namespace AIRCR{
   constexpr uint32_t REG_KEY { 0x05FAU }; 
   constexpr uint32_t BASE { 0xE000ED0CU };
    
   struct Register {volatile uint32_t BASE; }; 
   
   namespace Enable{
      constexpr uint32_t SYS_RESET = (REG_KEY << 16U) | // write key onto upper part of register
                              (7U << 8U) |// leave priority group unchanged
                              (1U << 2U); // set SYSRESETREQ bit
      }    
   
   namespace Run{
      inline void _DSB(){
        asm volatile("dsb 0xf":::"memory"); // data sync buffer, post a wait until key is written
      }
   }
}

static_assert(offsetof(AIRCR::Register, BASE)    == 0x00, "AIRCR at wrong offset");


namespace NVIC_ISER{ // interrupt set enable registers
   
   constexpr uint32_t BASE { 0xE000E100U };

   struct Register{
      uint32_t NOT_USING1[1];        
      volatile uint32_t ISER1;
   };
   
   namespace Enable{
      constexpr uint32_t USART2_Intrpt { 1U << 6U };
   
   }
}

inline RCC::Register*    rcc()    { return reinterpret_cast<RCC::Register*>(RCC::BASE);    }
inline GPIOA::Register*  gpioa()  { return reinterpret_cast<GPIOA::Register*>(GPIOA::BASE);  }
inline USART2::Register* usart2() { return reinterpret_cast<USART2::Register*>(USART2::BASE); }
inline FLASH::Register*  flash()  { return reinterpret_cast<FLASH::Register*>(FLASH::BASE);  }
inline AIRCR::Register* aircr(){ return reinterpret_cast<AIRCR::Register*>(AIRCR::BASE);  }
inline NVIC_ISER::Register* iser(){ return reinterpret_cast<NVIC_ISER::Register*>(
                                                               NVIC_ISER::BASE);  }



#endif
