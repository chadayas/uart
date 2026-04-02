#pragma once
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include<cstdint>



std::string read_from_port(int port, unsigned char* _buf);
void write_to_port(int port, uint8_t msg);
