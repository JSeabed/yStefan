import serial.tools.list_ports
import serial                   

#returns tty which is used by novatel chip
def getNRCPort():
    return portDefine(scanPorts())


def scanPorts():
    ports = list(serial.tools.list_ports.comports())
    port = list(serial.tools.list_ports.grep("09d7:0100"))[0][0]
    logger.debug("port is: " + port)
    return port



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
            filewrite(str(e)+"\n")
            logger.debug(str(e))

            port = 0
            #send an error message to the display
            commands.wrt_str("Trying to connect",5)
            #send an error message to the terminal
            print('\nUSB niet aangesloten\n')	
    return port
