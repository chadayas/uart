import serial
import os
import sys

BAUD_RATE = 115200
PORT = sys.argv[1]
BINARY = sys.argv[2]

BIN_LEN = os.path.getsize(BINARY)
DATA = open(BINARY , "rb").read()

# object creating using our agreed BR and user entered port

print(f"Opening serial connection at {PORT}") 
ser = serial.Serial(PORT, BAUD_RATE, timeout=5)

def recieve_serial():
    test_recv = ser.readline() 
    print(f"Recieved: {test_recv}")

def write_serial():
    ser.write(size.to_bytes(4, "little"))
    ser.write(DATA)
    print(f"Sent {BIN_LEN} bytes over serial to {PORT}")


def main():
    recieve_serial()
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
