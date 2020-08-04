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
    params = ["&", "2", "-4508.7", "-4508.7", "400",
            "015", "165", "2", "1", "1", "500", "0"]
    test = Connect()
    test.importParam(params)

    mode_num = input("Enter 1 for OCP, 2 for ASV: ")
    if(mode_num == '1'):
        test.readOCP()
        print(test.ocpData)
        print(test.ocp_average)
    if(mode_num == '2'):
        test.readData()
        plt.plot(test.xData, test.yData, 'r-')
        plt.title("Potential: {}, Scan Rate: {}, Gain: {}".format(params[3],
                    params[4], params[5]))
        plt.show()

main()
