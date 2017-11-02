import serial.tools.list_ports
#from log import *
import logging
import serial
import time			#time delay function imported

#logger = getLogger()
logger = logging.getLogger('dataManager')

class TrySerialExe():
    #returns tty which is used by novatel chip
    def getNRCPort():
    #Uncomment als je trySerial wilt vermijden
    #(als er geen connectie gemaakt kan worden met de novatel chip)
        return __portDefine(__scanPorts())
        return trySerial()


#Try to obtain port (tty/USBx).
#Return None if novatel is not found.
#Return port if dev/ttyUSBx is found.
    def __trySerial():
        attempt = 0
        for i in range(10):
            port = __portDefine(__scanPorts())
            if port is not None:
                print "USB found"
                return port
            if attempt is 9:
                logger.error("Failed to initialise port")
                return None
            attempt = attempt + 1
            logger.info("No connection established with novatel attempt: %d of the 10.", attempt)
            time.sleep(1)


    def __scanPorts():
        #ports = list(serial.tools.list_ports.comports())
        try:
            port = list(serial.tools.list_ports.grep("09d7:0100"))[1][0]
            print port
            logger.debug("port is: " + port)
            return port
        except Exception as e:
            logger.error(e)



#function to define the port the OEM7 is connected
    def __portDefine(PORT):
        try:
            #PORT = "/dev/ttyUSB1"
            #defining the serial port as a contant value
                print PORT
                port = serial.Serial(PORT, 9600)
            #logger.debug("Port is:" + port)
            #print("gevonden")
        except Exception, e:
            #print error
            #write out error to textdocument
                logger.debug(str(e))
                port = 0
            #send an error message to the display
            #send an error message to the terminal
                logger.error("Usb not found")
                return None
            return port
