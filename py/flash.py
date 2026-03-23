import serial
import io
import os
import sys
import time

BAUD_RATE = 115200
PORT = sys.argv[1]
BINARY = sys.argv[2]

BIN_LEN = os.path.getsize(BINARY)
DATA = open(BINARY , "rb").read()

MAX_ATTEMPTS = 5

# object creating using our agreed BR and user entered port
print(f"Opening serial connection at {PORT}") 
ser = serial.Serial(port=PORT, 
                    baudrate=BAUD_RATE, 
                    bytesize=serial.EIGHTBITS,
                    stopbits=serial.STOPBITS_ONE,
                    parity=serial.PARITY_NONE, 
                    timeout=5.0)
ser.reset_input_buffer()

sio = io.TextIOWrapper(ser, encoding='utf-8', newline='\n')

messages = []

def serial_handshake():
    attempts = 0 
    handshake = ser.read(size=1)
   # confirm we have recieved the byte from mcu 
    running = True 
    while running: 
        if handshake == b'\x7f':
            print("COMMS: recieved handshake from mcu") 
            rdy_byte = b'\x79' 
            ser.write(rdy_byte) 
            break
        if attempts == MAX_ATTEMPTS:
            running = False 
            raise TimeoutError("COMMS: haven't recieved byte, program exit") 
            break
        attempts+= 1

def recieve_serial():
    while True:
        line = ser.readline()
        if b'\x7f' not in line: 
            print(f"{line}") 
        break

def write_serial():
    ser.write(BIN_LEN.to_bytes(4, "little"))
    ser.write(DATA)
    print(f"Sent {BIN_LEN} bytes over serial to {PORT}")

def main():
    serial_handshake() 
    recieve_serial()
    recieve_serial()
    write_serial()
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
