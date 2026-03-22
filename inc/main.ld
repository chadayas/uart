

INPUT(main.o flash.o usart.o startup.o stub.o)

MEMORY {

	SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
	SRAM2 (rwx) : ORIGIN = 0x2001C000, LENGTH = 16K
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
}


_estack = ORIGIN(SRAM1) + LENGTH(SRAM1);

SECTIONS {

.text : {
	KEEP(*(.isr_vector))   /* Vector table at start of flash */
    *(.text*)              /* All other code */
    *(.rodata*)            /* Read-only constants */
  } > FLASH


  .data :
  {
    *(.data*)              /* Initialized global/static variables */
  } > SRAM1 AT> FLASH


  .bss :
  {
    *(.bss*)               /* Uninitialized global/static variables */
    *(COMMON)
  } > SRAM1





}

