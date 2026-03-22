#include "reg.h"

// functions for register checks, bits of the mmr help tell
// us the status of what is inside of the register

inline void transmit_reg_empty_check();

inline void delay();

inline void read_reg_empty_check();

inline void flash_bsy_wait();
