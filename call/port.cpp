#include"port.h"

// Open the serial port. Change device path as needed (currently set to an standard FTDI 
// USB-UART cable type device)
 
int serial_port = open("/dev/ttyACM0", O_RDWR);

void read_from_port(int port, unsigned char *_buf){
   ssize_t num_bytes = read(port, &_buf, sizeof(_buf));
   std::cout << "Read " << num_bytes << " from serial port" << std::endl; 
   std::string msg(_buf); 
   std::cout << msg << std::endl;
}

void write_to_port(int port, uint8_t msg){
  ssize_t num_write = write(port, &msg, 1);
  std::cout << "Writing" << num_write << " to serial port" << std::endl; 
}

int main() {
   port_init();  
    
   // Write to serial port
  uint16_t msg = 0x80;
  //write(serial_port, &msg, 1);
   printf("Writing %i bytes into port\n",1);
  // Allocate memory for read buffer, set size according to your needs
  unsigned char read_buf[128];

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be 
  // -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
  } else if (num_bytes == 0){
     printf("nothing was read\n");
      return 1; 
  }

  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, 
  // don't try and
  // print it to the screen like this!)
  
  printf("Read %i bytes\nReceived message: %i \n", num_bytes, static_cast<int>(*read_buf));

  close(serial_port);
  return 0; // success
};
