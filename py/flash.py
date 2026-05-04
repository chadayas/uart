import serial
import os
import sys
import time
import io

BAUD_RATE = 115200
PORT = sys.argv[1]
BINARY = sys.argv[2]

BIN_LEN = os.path.getsize(BINARY)
DATA = open(BINARY, "rb").read()

print(f"HOST: opening serial connection at {PORT}")
ser = serial.Serial(port=PORT,
                    baudrate=BAUD_RATE,
                    bytesize=serial.EIGHTBITS,
                    stopbits=serial.STOPBITS_ONE,
                    parity=serial.PARITY_NONE,
                    timeout=5.0,
                    xonxoff=False,
                    rtscts=False,
                    dsrdtr=True)
ser.reset_input_buffer()

sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))


def read_serial():
    # read serial for string sends from mcu 
    msg_read = sio.readline().strip()
    print(msg_read)


def send_reset_byte():
    rst_byte = b'\x80' 
    print("HOST: prompting hardware reset") 
    ser.write(rst_byte)


def wait_ack():
    # check for confirmation from mcu before moving on, rasie error if wrong 
    b = ser.read(1)
    if b != b'\x79':
        raise RuntimeError(f"expected ACK 0x79, got {b}")

def serial_handshake():
    while True:
        b = ser.read(1)
        if b == b'\x7f':
            ser.write(b'\x79')
            print("HOST: handshake complete")
            ser.reset_input_buffer()
            break

def wait_ready():
    b = ser.read(1)
    if b != b'\x7e':
        raise RuntimeError(f"expected ready 0x7E, got {b}")
    print("HOST: MCU ready, sending length")

def write_serial():
    # wait for MCU ready signal before sending length
    wait_ready()
    ser.write(BIN_LEN.to_bytes(4, "little"))

    read_serial() 
    
    # wait for erase complete
    wait_ack()
    print("HOST: erase done, sending binary")

    # send binary in 32-byte chunks, wait for ACK after each
    total = 0
    for i in range(0, BIN_LEN, 32):
        chunk = DATA[i:i+32]
        ser.write(chunk)
        wait_ack()
        total += len(chunk)
        print(f"HOST: {total}/{BIN_LEN} bytes written", end='\r', flush=True)
        time.sleep(0.005)
    read_serial()

def main():
    send_reset_byte() 
    serial_handshake()
    write_serial()
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
