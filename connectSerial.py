import serial.tools.list_ports
from log import getLogger
import serial                   

logger = getLogger()

#returns tty which is used by novatel chip
def getNRCPort():
    return trySerial()

def trySerial():
    attempt = 0
    for i in range(10):
        port = portDefine(scanPorts())
        if port is not None:
            return port
        if attempt is 10:
            logger.error("Failed to initialise port")
            return None
        attempt = attempt + 1
        logger.info("No connection established with novatel attempt: %d of the 10.", attempt)
        sleep(1)


def scanPorts():
    #ports = list(serial.tools.list_ports.comports())
    try:
        port = list(serial.tools.list_ports.grep("09d7:0100"))[0][0]
        logger.debug("port is: " + port)
        return port
    except Exception as e:
        logger.error(e)



#function to define the port the OEM7 is connected
def portDefine(PORT):
    try:	
            #PORT = "/dev/ttyUSB1"	
            #defining the serial port as a contant value
            port = serial.Serial(PORT, 9600)
            logger.debug("Port is:" + port)
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
