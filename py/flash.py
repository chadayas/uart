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

messages = []

def recieve_serial():
    while True:
        line = ser.readline()
        if not line:
            break
        messages.append(line.decode('utf-8').strip())

def write_serial():
    print("Start write\n")
    ser.write(BIN_LEN.to_bytes(4, "little"))
    ser.write(DATA)
    print(f"Sent {BIN_LEN} bytes over serial to {PORT}")

def main():
    recieve_serial()
    write_serial()
    recieve_serial()
    for msg in messages:
        print(msg)
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
