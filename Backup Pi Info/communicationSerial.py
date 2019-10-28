import serial
import time

data = {}
index = 0

port = serial.Serial(
    '/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)
reading = True
while reading:
    x = port.readline().decode("utf-8")
    if x is None:
        print("qweqwe")
        port.close()
        reading = False
    data[index] = x
    print(data[index])
    index += 1

        

print("Voici la ligne 3")
print(data[3])