import serial
import os
import sys

BAUD_RATE = 115200

port = sys.argv[1]
bin_file = sys.argv[2]

size = os.path.getsize(bin_file)
data = open(bin_file, "rb").read()

ser = serial.Serial(port,BAUD_RATE, timeout=5)

ser.write(size.to_bytes(4, "little"))
ser.write(data)
ser.close()

print(f"Sent {size} bytes over serial")

