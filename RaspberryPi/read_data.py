def set_parameter(some_input_from_ML):
    '''
    mode     = 0          vOffset      = 5
    vInit    = 1      initialDelay = 6
    vFinal   = 2      nRuns        = 7
    scanRate = 3      logIvl       = 8
    cGain    = 4      del2         = 9
    '''
    # ASV mode param is 2
    param_list = ['2']
    
    # need to add other variables


def getData(ser_device, param_list):
    ## Start measurement
    ser_device.write('&'.encode('utf-8'))
    for param in param_list:
        ser_device.write('param'.encode('utf-8'))
    
    ## some adjustment need to be done to read data properly
    ser_device.read(size=100)


def processData(raw_output):
    return 0