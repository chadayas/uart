
namespace RCC {
   constexpr uint32_t BASE { 0x40023800 };
   
   struct Register{
      uint32_t NOT_USING1[12]; 
      volatile uint32_t AHB1ENR_REG; // offset 0x30 
      uint32_t NOT_USING2[3]; 
      volatile uint32_t APB1ENR_REG; // offset 0x40
   };

   namespace Enable{
      constexpr uint32_t GPIOA { (1 << 0) };
      constexpr uint32_t USART2 { (1 << 17) };
   }     
}

namespace GPIOA {
   constexpr uint32_t BASE { 0x40020000 };
   
   struct Register{
      volatile uint32_t MODER_REG; // offset 0x00 
      uint32_t NOT_USING[7];
      volatile uint32_t AFRL_REG; // offset 0x20
   };

   namespace Set{
      constexpr uint32_t PA2_AF_MODE { (1 << 5) };
      constexpr uint32_t PA3_AF_MODE { (1 << 7) };
      
      constexpr uint32_t USART2_TX { (1 << 10) | (1 << 9) | (1 << 8) };
      constexpr uint32_t USART2_RX { (1 << 14) | (1 << 13) | (1 << 12) };
   }     
}


namespace USART2 {
   constexpr uint32_t BASE { 0x40004400 };
   
   struct Register{
      volatile uint32_t STATUS_REG; // offset 0x00 
      volatile uint32_t DATA_REG; // offset 0x04
      volatile uint32_t BAUDRATE_REG; // offset 0x08
      volatile uint32_t CTRL_REG; // offset 0x0C
   };

   namespace Enable{
      constexpr uint32_t USART { (1 << 13) };
      constexpr uint32_t TX_PIN { (1 << 3) };
      constexpr uint32_t RX_PIN { (1 << 2) };
       
   }     
   
   namespace Set{
      constexpr uint32_t BAUD_RATE = (8 << 4) | 11; // 115200 baudrate 
   } 
   
   namespace Status{
     
      inline uint32_t RDR_NOT_EMPTY() {
        USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
        return reg->STATUS_REG & (1 << 5);
      }  
      
      inline uint32_t TDR_IS_EMPTY() {
         USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
         return reg->STATUS_REG & (1 << 7);
      }  
      
      inline uint32_t TRANSMIT_COMPLETE(){
         USART2::Register* reg = reinterpret_cast<USART2::Register*>(USART2::BASE);
         return reg->STATUS_REG & (1 << 6);
      }  
    
   } 
}


namespace FLASH{
   constexpr uint32_t BASE { 0x40023C00 };
   
   constexpr uint32_t KEY1 = 0x45670123; 
	constexpr uint32_t KEY2 = 0xCDEF89AB; 

   struct Register{
      uint32_t NOT_USING1[1]; 
      volatile uint32_t KEY_REG; 
      uint32_t NOT_USING2[1]; 
      volatile uint32_t STATUS_REG; 
      volatile uint32_t CTRL_REG; 

   };

   namespace Enable{
      constexpr uint32_t SECTOR_ERASE{ (1 << 1) };
      constexpr uint32_t ERASE { (1 << 16) }; 
   }     
   namespace Clear{
      constexpr uint32_t SNB_BITS{ ~(0xF << 3) };
      constexpr uint32_t SER_BIT {~(1 << 1)}; 
   }
   namespace Set{
      constexpr uint32_t sector_2 { (1 << 4) };
      constexpr uint32_t sector_3 { (1 << 4) | (1 << 3) };
      constexpr uint32_t sector_4 { (1 << 5) };
   }
   
   namespace Status{
      inline uint32_t FLASH_BUSY(){ 
         FLASH::Register* reg = reinterpret_cast<FLASH::Register*>(FLASH::BASE);
         return reg->STATUS_REG & (1 << 16);
      }
   }

}

extern RCC::Register* rcc;  
extern GPIOA::Register* gpioa; 
extern USART2::Register* usart2;
extern FLASH::Register* flash;

