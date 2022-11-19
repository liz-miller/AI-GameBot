import serial
import time

arduino = serial.Serial(port = "/dev/cu.usbmodem14201", timeout=0)
time.sleep(2)

# get the position, encode it, write to arduino
setpos = 3
setpos = str(setpos)
arduino.write(setpos.encode())
#time.sleep(1)


# while True:
#
#     #manually enter
#     print ("Which NeoPixel Position to light?")
#
#     pos = input()
#     print ("You Entered:", pos)
#
#     arduino.write(pos.encode())

    # var = str(input())
    # print ("You Entered :", var)
    #
    # if(var == '1'):
    #     arduino.write(str.encode('1'))
    #     print("LED turned on")
    #     time.sleep(1)
    #
    # if(var == '0'):
    #     arduino.write(str.encode('0'))
    #     print("LED turned off")
