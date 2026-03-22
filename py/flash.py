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

# object creating using our agreed BR and user entered port
print(f"Opening serial connection at {PORT}") 
ser = serial.Serial(port=PORT, 
                    baudrate=BAUD_RATE, 
                    bytesize=serial.EIGHTBITS,
                    stopbits=serial.STOPBITS_ONE,
                    parity=serial.PARITY_NONE, 
                    timeout=5.0)

sio = io.TextIOWrapper(ser, encoding='utf-8', newline='\n')

messages = []

def serial_handshake():
    handshake = sio.readline()
   # confirm we have recieved the byte from mcu 
    if handshake == "mcu_rdy":
        print("recieved handshake from mcu") 
        byte = "host_rdy" 
        ser.write(byte.endcode("utf-8")) 
        recieve_serial()

def recieve_serial():
    while True:
        line = sio.readline()
        print(f"added: {line}") 
        messages.append(line.strip())
        if line == "handshake\n":
            break

def write_serial():
    ser.write(BIN_LEN.to_bytes(4, "little"))
    ser.write(DATA)
    print(f"Sent {BIN_LEN} bytes over serial to {PORT}")

def main():
    recieve_serial()
    for msg in messages:
        print(msg)
    write_serial()
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
