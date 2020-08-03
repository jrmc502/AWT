from connect import Connect
import matplotlib.pyplot as plt
import numpy as np

'''
    mode     = 0      vOffset      = 5
    vInit    = 1      initialDelay = 6
    vFinal   = 2      nRuns        = 7
    scanRate = 3      logIvl       = 8
    cGain    = 4      del2         = 9
    '''

def main():
    params = ["&", "9", "-300", "800", "400",
            "015", "165", "2", "1", "1", "500", "0"]
    test = Connect()
    test.importParam(params)
    # test.readData()
    # plt.plot(test.xData, test.yData, 'ro')
    # plt.show()


    test.readOCP()
    print(test.ocpData)
    print(test.ocp_average)

main()
