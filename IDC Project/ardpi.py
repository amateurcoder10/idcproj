import bluetooth
import time

bd_addr = "30:15:01:27:04:86"
port = 1
sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
sock.connect((bd_addr,port))

while 1:
    time.sleep(5)
    buffer = sock.recv(4096)
    print buffer
sock.close()
