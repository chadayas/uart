#include "reg.h"
// flash functions


void flash_init();
void flash_write(uint32_t dest, uint32_t len);
void flash_erase();
