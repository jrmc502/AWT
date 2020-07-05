from connect import Connect
import matplotlib.pyplot as plt
import numpy as np

def main():
    params = ["&", "2", "-300", "800", "400",
            "015", "165", "2", "1", "1", "500", "0"]
    test = Connect()
    test.importParam(params)
    # print(test.ser.read(size=1000))
    test.readData()
    print(len(test.xData))
    print(len(test.yData))
    plt.plot(test.xData, test.yData, 'ro')
    plt.show()

main()
