#include "reg.h"

void uart_send_string(const char* msg);
void uart_send_byte(uint8_t byte);

void open_USART_config();
void start_transmission();
void start_recieve();
