import serial
from serial.tools.list_ports import comports
import time

def setup_connection():
    # Get a list of all connected devices through serial port
    com_devices = comports()

    # Connect to the AWT kit. Index might need to change.
    ser = serial.Serial(com_devices[1][0],timeout=1)

    ## Test connection
    print(ser.name)
    ser.write('*'.encode('utf-8'))
    time.sleep(.1)
    connection_status_char = ser.read(size=1)

    # should return a '&'
    if(connection_status_char == '&'):
        print("Connection established.")
    else:
        print("The device says: " + connection_status_char)


    # ## Test Read Data
    # params = ["&","1","-300","800","400","015","165","2","1","1","500","0"]
    # for p in params:
    #     ser.write(p.encode('utf-8'))
    # print(ser.read(size=1000))