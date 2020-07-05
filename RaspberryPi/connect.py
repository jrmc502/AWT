import serial
from serial.tools.list_ports import comports
import time

class Connect:
    connection_status = False

    def __init__(self):
        self.xData = []
        self.yData = []
        # Get a list of all connected devices through serial port
        com_devices = comports()

        # Connect to the AWT kit. Index might need to change.
        ser = serial.Serial(com_devices[1][0],timeout=10)
        ## Test connection
        print(ser.name)
        ser.write('*'.encode('utf-8'))
        time.sleep(.1)
        device_response = ser.read(size=1).decode('utf-8')

        # should return a '&'
        if(device_response == '&'):
            print("Connection established.")
            # connection_status = True
            self.ser = ser
        else:
            print("The device says: " + device_response)

    # setup_connection()

    def importParam(self, param_list):
        '''
        mode     = 0      vOffset      = 5
        vInit    = 1      initialDelay = 6
        vFinal   = 2      nRuns        = 7
        scanRate = 3      logIvl       = 8
        cGain    = 4      del2         = 9
        '''
        for p in param_list:
            self.ser.write(p.encode('utf-8'))
            self.ser.write(','.encode('utf-8'))
            time.sleep(.1)
        
    def readData(self):
        # read headers
        header_line = self.ser.read_until().decode('utf-8')
        print(header_line)
        
        while(header_line != '$\r\n'):
            header_line = self.ser.read_until().decode('utf-8')
            print(header_line)
        print('-------------------------')

        ## record actual data
        data_list = [""]
        while True:
            data_line = self.ser.read_until().decode('utf-8')
            data_list = data_line.split(",")
            ## check if the run ends
            if(data_list[0] == '99999'):
                break

            xRead = float(data_list[0])
            yRead1 = float(data_list[1])/1000
            yRead2 = float(data_list[2])/1000
            yRead = yRead1 - yRead2

            mVmin = float(data_list[3])
            mVmax = float(data_list[4])

            # record data
            self.xData.append(xRead)
            self.yData.append(yRead)
