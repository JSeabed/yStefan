import serial.tools.list_ports
#from log import *
import logging
import serial
import time			#time delay function imported

#logger = getLogger()
logger = logging.getLogger('dataManager')

class TrySerialExe():
    def __init__(self, baudrate):
        #self.poortnmbr = poortnmbr
        self.baudrate = baudrate

    #returns tty which is used by novatel chip 1
    def getNRCPort():
    #Uncomment als je trySerial wilt vermijden
    #(als er geen connectie gemaakt kan worden met de novatel chip)
        return portDefine(scanPorts())
        #return trySerial()
        #kjhlhp;

    def scanPorts(self):
        #ports = list(serial.tools.list_ports.comports())
        try:
            self.poortnmbr = list(serial.tools.list_ports.grep("09d7:0100"))[1][0]
            print self.poortnmbr
            logger.debug("port is: " + self.poortnmbr)
            return self.poortnmbr
        except Exception as e:
            logger.error(e)
            self.poortnmbr = None
            return self.poortnmbr

#function to define the port the OEM7 is connected
    def portDefine(self):
        try:
            #PORT = "/dev/ttyUSB1"
            #defining the serial port as a contant value
            print self.poortnmbr
            self.baudrate = serial.Serial(self.poortnmbr, 9600)
            #logger.debug("Port is:" + port)
            #print("gevonden")
        except Exception, e:
            #print error
            #write out error to textdocument
            logger.debug(str(e))
            self.poortnmbr = None
            #send an error message to the display
            #send an error message to the terminal
            logger.error("Usb not found")
            #return None
            return self.poortnmbr


#Try to obtain port (tty/USBx).
#Return None if novatel is not found.
#Return port if dev/ttyUSBx is found.
#    def trySerial(self):
#        attempt = 0
#        for i in range(10):
#            port = _portDefine(_scanPorts())
#            if port is not None:
#                print "USB found"
#                print port
#                return self.poortnmbr
#                return port
#            if attempt is 9:
#                logger.error("Failed to initialise port")
#                return None
#            attempt = attempt + 1
#            logger.info("No connection established with novatel attempt: %d of the 10.", attempt)
#            time.sleep(1)
