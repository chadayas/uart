#include "reg.h"

// functions for handling uart peripheral

void uart_send_proto(uint8_t byte);
void uart_send_string(const char* msg);
void wait_for_ack(uint8_t send_byte, uint8_t expect_byte);

void open_USART_config();
void start_transmission();
void start_recieve();
