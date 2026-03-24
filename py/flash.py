import serial
import os
import sys

BAUD_RATE = 115200
PORT = sys.argv[1]
BINARY = sys.argv[2]
TAG = "HOST"
BIN_LEN = os.path.getsize(BINARY)
DATA = open(BINARY, "rb").read()

print(f"Opening serial connection at {PORT}")
ser = serial.Serial(port=PORT,
                    baudrate=BAUD_RATE,
                    bytesize=serial.EIGHTBITS,
                    stopbits=serial.STOPBITS_ONE,
                    parity=serial.PARITY_NONE,
                    timeout=5.0)
ser.reset_input_buffer()

# read one framed message from MCU
# MCU sends: [0x01][byte] for protocol, [0x02][...string...\n] for debug
def read_frame():
    type_byte = ser.read(1)
    if not type_byte:
        raise TimeoutError(f"{TAG} cannot decipher type_byte:{type_byte}") 
    t = type_byte[0]
    if t == 0x01:
        proto = ser.read(1)
        return ('proto', proto[0] if proto else None)
    elif t == 0x02:
        msg = b''
        while True:
            b = ser.read(1)
            if not b or b == b'\n':
                break
            msg += b
        return ('debug', msg.decode('utf-8', errors='replace'))
    return (None, None)

# block until MCU sends expected proto byte, printing any debug strings along the way
def wait_for_proto(expect_byte):
    while True:
        kind, value = read_frame()
        if kind == 'proto' and value == expect_byte:
            return
        elif kind == 'debug':
            print(f"INFO bootloader: {value}")

# echo a raw byte back to MCU to break its wait_for_ack loop
def sync(byte):
    ser.write(bytes([byte]))

def serial_handshake():
    # initial sync — raw 0x7F from MCU, raw 0x79 reply from host (no type prefix)
    while True:
        b = ser.read(1)
        if b == b'\x7f':
            print(f"{TAG}: received handshake from MCU {b}")
            ser.write(b'\x79')
            print(TAG +": host is ready")
            break

def write_serial():
    # send firmware size (4 bytes little endian)
    ser.write(BIN_LEN.to_bytes(4, "little"))

    # gate 1: MCU received length, about to erase
    wait_for_proto(0x05)
    sync(0x05)

    # gate 2: MCU erase done, ready for binary
    wait_for_proto(0x06)
    sync(0x06)

    # stream binary
    ser.write(DATA)
    print(f"Sent {BIN_LEN} bytes over serial to {PORT}")

    # gate 3: MCU write complete
    wait_for_proto(0x07)
    sync(0x07)

def main():
    serial_handshake()
    write_serial()
    ser.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
