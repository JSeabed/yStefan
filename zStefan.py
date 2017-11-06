#Written by Stefan van Delft 26/07/2017
#Display is added by Martijn Rombouts



#mattie regex
regexIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"			#this is used to filter out the IP adress


import time			#time delay function imported
import datetime
import usb.core
import usb.util
import re
import commands
import RPi.GPIO as GPIO
import os
import log
import errno
import select
from collections import namedtuple

logger = log.setup_custom_logger('dataManager')

from connectSerial import getNRCPort

#Used for debugging.
#import logger
#logger.basicConfig(stream=sys.stderr)
#exapmle
#logger = getLogger()
#logger.debug('A debug message!')
#logger.info('We processed %d records', len(processed_records))


usleep = lambda x: time.sleep(x/1000000.0)

#ID's
IP_ID = "0: "
GPGGA_ID = "1: "
INS_ID = "1: "


def fifoPort(pipeIn):
	FIFO = '/tmp/mypipe'
        logger.debug("Child: preparing fifo\n")
	try:
	    os.mkfifo(FIFO)
	except OSError as oe:
	    if oe.errno != errno.EEXIST:
	        raise

        fifo = os.open(FIFO, os.O_WRONLY)
        while True:
            logger.debug("Child checking FD")
            r, _, _ = select.select([pipeIn], [], [], )
            if not r:
                #no data
                logger.debug("No data in FD")
                #print "No data in FD\n"
                pass
            else:
                #data available
                data =  os.read(pipeIn, 1024)
                #with open(FIFO, "w", 1) as fifo:
                os.write(fifo, data + '\n')
                #fifo.flush()
                    #fifo.write("\0")
        fifo.close()


def filewrite(rcv):                             		#Function to write data to a .txt file
	logfile = open("templog.txt", "a")      			#open file
	logfile.write(str(datetime.datetime.now()) + "\n")	#adding time of error
	logfile.write(rcv)                      			#write line in file
	logfile.close                           			#close file


def readSerial(port):							#reading all the data that is send by the receiver. everything that gets send is added together
#Read serial
	try:		    									#testing if data is transmitted
		data = {'ip': None, \
                        'gpgga': None, \
                        'ins_active': None, \
                        'ins_inactive': None, \
                        'ins_aligning': None, \
                        'ins_high_variance': None, \
                        'ins_solution_good': None, \
                        'ins_solution_free': None, \
                        'ins_alignment_complete': None, \
                        'determining_orientation': None, \
                        'waiting_initialpos': None, \
                        'waiting_azimuth': None, \
                        'initializing_biases': None, \
                        'motion_detect': None, \
                        'finesteering': None, \
                        'coarsesteering': None, \
                        'unknown': None, \
                        'aproximate': None, \
                        'coarseadjusting': None, \
                        'coarse': None, \
                        'freewheeling': None, \
                        'fineadjusting': None, \
                        'fine': None, \
                        'finebackupsteering': None, \
                        'sattime': None, \
                        'gphdt': None, \
                        'ins': None}					#define what to expect in the dictionary


                #print "Ik kom hier 4"
		j = 0
		rcv = [None]*25
		for x in range (0, 25):
			rcv[j] = port.readline()                 #rvc is the serial data received
			j = j + 1
		#print("----------------------------------------\n")					#adding a line in the terminal for transparity
		#print("----------------------------------------\n")					#adding a line in the terminal for transparity
                #print cData
		str1 = ''.join(rcv)
		#print rcv
		for word in str1.split():
			m = re.search(regexIP, str1)								#let the regex filter out the ip of the text that was send
			if(m is not None and data['ip'] is None):
				data['ip'] = "[0]" + m.group()									#adding IP to the dictionary
			if(exact_Match(word,"FINESTEERING") and data['finesteering'] is None):			#
				data['finesteering'] = True								#adding finesteering to the dictionary
			if(exact_Match(word,"COARSESTEERING") and data['coarsesteering'] is None):
				data['coursesteering'] = True							#adding coursesteering to the dictonary
			if(exact_Match(word,"UNKNOWN") and data['unknown'] is None):				#
				data['unknown'] = True
			if(exact_Match(word,"APROXIMATE") and data['aproximate'] is None):			#
				data['aproximate'] = True
			if(exact_Match(word,"COARSEADJUSTING") and data['coarseadjusting'] is None):		#
				data['coarseadjusting'] = True
			if(exact_Match(word,"COARSE") and data['coarse'] is None):				#
				data['coarse'] = True
			if(exact_Match(word,"FREEWHEELING") and data['freewheeling'] is None):			#
				data['freewheeling'] = True
			if(exact_Match(word,"FINEADJUSTING") and data['fineadjusting'] is None):		#
				data['fineadjusting'] = True
			if(exact_Match(word,"FINE") and data['fine'] is None):					#
				data['fine'] = True
			if(exact_Match(word,"FINEBACKUPSTEERING") and data['finebackupsteering'] is None):	#
				data['finebackupsteering'] = True
			if(exact_Match(word,"SATTIME") and data['sattime'] is None):				#
				data['sattime'] = True
                        if(exact_Match(word,"INS_ACTIVE") and data['ins_active'] is None):
                                data['ins_active'] = True
                        if(exact_Match(word,"INS_INACTIVE") and data['ins_inactive']is None):
                                data['ins_inactive'] = True
                        if(exact_Match(word,"INS_ALIGNING") and data['ins_aligning'] is None):
                                data['ins_aligning'] = True
                        if(exact_Match(word,"INS_HIGH_VARIANCE") and data['ins_high_variance'] is None):
                                data['ins_high_variance'] = True
                        if(exact_Match(word,"INS_SOLUTION_GOOD") and data['ins_solution_good'] is None):
                                data['ins_solution_good'] = True
                        if(exact_Match(word,"INS_SOLUTION_FREE") and data['ins_solution_free'] is None):
                                data['ins_solution_free'] = True
                        if(exact_Match(word,"INS_ALIGNMENT_COMPLETE") and data['ins_alignment_complete'] is None):
                                data['ins_alignment_complete'] = True
                        if(exact_Match(word,"DETERMINING_ORIENTATION") and data['determining_orientation'] is None):
                                data['determining_orientation'] = True
                        if(exact_Match(word,"WAITING_INITIALPOS") and data['waiting_inititalpos'] is None):
                                data['waiting_initialpos'] = True
                        if(exact_Match(word,"WAITING_AZIMUTH") and data['waiting_azimuth'] is None):
                                data['waiting_azimuth'] = True
                        if(exact_Match(word,"INITIALIZING_BIASES") and data['initializing_biases'] is None):
                                data['initializing_biases'] = True
                        if(exact_Match(word,"MOTION_DETECT") and data['motion_detect'] is None):
                                data['motion_detect'] = True
			if(findWord(word,"GPGGA") and data['gpgga'] is None):				#getting GPGGA out of the read values
				mylist = word.split(',')						#split up the line in which GPGGA was found
				data['gpgga'] = mylist							#add GPGGA to the dictionary
			if(findWord(word,"INS_") and data['ins'] is None):				#getting INS out of the read values
				mylist2 = word.split(',')						#split up the line in which INS was found
				data['ins'] = mylist2							#add INs to the dictionary
			#print "testoe"
			if(findWord(word,"GPHDT,")and data['gphdt'] is None):
				data['gphdt'] = word.split(',')							#add INs to the dictionary
				(data['gphdt'][0]) = (data['gphdt'][0]).split('$')
				#data['gphdt'] = True
			#if("$GPHDT" in rcv):
			#	split_GPHDT = rcv.split(',')
			#	print(split_GPHDT)
			#	if (split_GPHDT[1] >= '0'):
			#		print("heading")
					#commands.wrt_str("Ok",4)
			#	else:
					#commands.wrt_str("Non",4)
			#		return
			#print "end filter"
                #print data to terminal
		#displayData(data)									#call displayData def / sents one outcome to child


               # os.write(pipeOut, data['ip'])

		#port.close()										#close port to OEM7
		#time.sleep(2)										#add a delay of 2 seconds
		#fifoPort((data['ip']))
        #print "Parent: writing data to child through FD\n"
                #logger.debug("Parent: writing data to child through FD\n")
		#print data

		time.sleep(1)
                return data
        #write to the fifo pipe (to genieInterface)
                #os.write(pipeOut, "0: " + data['ip'])


	except Exception, e:							#not receiving data from OEM7
		#print error
		filewrite(str(e)+"\n")						#write out error to text file
		port = 0									#define port as 0
		#commands.wrt_str("Connection Error",2)		#write an error message to display
		#print('\nUSB kan niet uitgelezen worden\n')	#write an error message to terminal
                #print "Ik kom hier 1"
		print str(e)
		#time.sleep(1)								#put 10 second delay in before repeating try functions



def printData(data):						#def that prints data to the terminal, used to check for problems
	print(data['gpgga'][6])					#print dictionary adress 6 in gpgga subclass
	print(data['gpgga'][7])					#print dictionary adress 7 in gpgga subclass
	print(data['ip'])						#print dictionary ip
	print(data['finesteering'])				#print dictionary finesteering
	return

def headingGPHDT(data):
	print data('gphdt')
	try:
            if ((data['gphdt'][0]) is 'GPHDT'):
                mode = "[4]" + "OK"
	    else:
		mode = "[4]" + "Non"
            return mode
        except Exception as e:
            print str(e)


def displayData(data):						#this def tests for 1 of 11 options
				#for each of the modes stated here take up the same place in the string that gets passed by the receiver
	#Sattalites
	#commands.wrt_str(data['gpgga'][7],7)			#write out the amount of sattalites are in contact with OEM7

	IP_String = bytearray()						#converting IP string to a byte array
	#IP_String.extend(" ")
	IP_String.extend(data['ip'])
	#commands.wrt_str(IP_String,1)				#write out the IP adress to the first string adress on the display
	#commands.wrt_str(data['ip'])

	if (data['finesteering'] == True):			#testing for finesteering
	        mode = "[1]" + "Fine steering"
	elif (data['coarsesteering'] == True):			#testing for coarsesteering
		mode = "[1]" + "Coarse steering"			#write out 'Coarse' to the 5th string adress on the display
        elif (data['unknown'] == True):				#
		mode = "[1]" + "Unknown"
	elif (data['aprocimate'] == True):
		mode = "[1]" + "Aproximate"
        elif (data['coarseadjusting'] == True):
		mode = "[1]" + "Coarse adjusting"
        elif (data['coarse'] == True):
		mode = "[1]" + "Coarse"
        elif (data['freewheeling'] == True):
		mode = "[1]" + "Freewheeling"
        elif (data['fineadjusting'] == True):
		mode = "[1]" + "Fineadjusting"
        elif (data['Fine'] == True):
		mode = "[1]" + "Fine"
        elif (data['finebackupsteering'] == True):
		mode = "[1]" + "Fine backupsteering"
        elif (data['sattime'] == True):
		mode = "[1]" + "sattime"

	return mode


def tryIns(data):										#def to determine INS value. in order to keep track of this value we asign the identifier [2]
	try:												#try to define, if failed goes to except
		partup = (data['ins'][20])						#define dictionary entry 20 from ins as partup for further filtering
		clean_Ins = partup.split('*')					#split up partup, use * as the separator
		data['insclean'] = clean_Ins					#add the split entry's as seperate dictionary adresses
		#print(data['insclean'][0])						#print the wanted dictionary adress to the terminal for control
		if (data['ins_active'] == True):				#check library if ins active is true
			mode = "[2]" + "Ins active"			#write to display on adress 2 of the string list
		elif (data['ins_aligning'] == True):			#check library if aligning is true
			mode = "[2]" + "Ins aligning"       			#write out only if aligning is true
		elif (data['ins_high_variance'] == True):		#check library if high variance is true
			mode = "[2]" + "Ins high variance"              	#write out only if above check passes
		elif (data['ins_solution_good'] == True):		#check if solution good is true
			mode = "[2]" + "Ins solution good"       		#write out only if above check passes
		elif (data['ins_solution_free'] == True):		#check if solution free is true
			mode = "[2]" + "Ins solution free"      		#write out only if above check passes
		elif (data['ins_alignment_complete'] == True):		#check if alignment is complete
			mode = "[2]" + "Ins alignment complete"         	#
		elif (data['determining_orientation'] == True):		#
			mode = "[2]" + "Determining orientation"        	#
		elif (data['waiting_initialpos'] == True):		#
			mode = "[2]" + "Waiting initialpos"             	#
		elif (data['waiting_azimuth'] == True):			#
			mode = "[2]" + "Waiting azimuth"        		#
		elif (data['initializing_biases'] == True):		#
			mode = "[2]" + "Initializing biases"            	#
		elif (data['motion_detect'] == True):			#
			mode = "[2]" + "Motion detect"          		#
		else:											#when INS is inactive
			mode = "[2]" + "Ins inactive"   			#write to display on adress 2 of the string list
			return mode
	except Exception, e:								#error handling INS testing
		#print error
		filewrite(str(e)+"\n")							#write error to text file
		print (str(e))
		print (data['ins_active'])									#write error to the terminal
                #print "Ik kom hier 2"


def dataManager(data ,pipeOut):
    #fill list for fifo
    try:
            #create list
            sendList = [None]*5
            #add IP
            sendList[0] = (data['ip'])
            #add status
            sendList[1] = tryIns(data)							#call on tryIns function
            #add
	    sendList[2] = ("[7]" + data['gpgga'][7])
            #add
            sendList[3] = statusGPGGA(data)
            #add
            sendList[4] = displayData(data)
            #sendList[2] = statusGPGGA(data, pipeOut)								#call statusGPGGA def / sents one outcome to child
    except Exception as e:
            logger.error(str(e))
    #printData(data)
    #call exportData def / sents one outcome to child

    print "i is: \n"
    try:
            for i in sendList:
                    os.write(pipeOut, i)
                    usleep(275)
    except Exception as e:
            logger.error(str(e))



def findWord(phrase, word):								#word seacher that is used by readSerial
	if(phrase.find(word) > 0):							#testing for an exact match
		return True										#return true to confirm the word
	return False


def exact_Match(phrase, word):						#exact match def for filtering words
    b = r'(\s|^|$)'
    res = re.match(b + word + b, phrase, flags=re.IGNORECASE)
	#print res
    return bool(res)


def statusGPGGA(data):						#used to determine the status for GPGGA by reading a number out of the input string. the defenition for each number can be found in Novatel's manual
	try:											#determine the gpgga status
		if (data['gpgga'][6]) is '0':				#mode 0 of gpgga, represents "No fix"
			mode = "[3]" + "No fix"					#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '1':				#mode 1 of gpgga, represents "Single point"
                        mode = "[3]" + "Single point"	#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '2':				#mode 2 of gpgga, represents "Pseudorange"
			mode = "[3]" + "Pseudorange"			#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '3':				#mode 3 of gpgga, represents nothing according to Manual
			mode = "[3]" + ". . ."					#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '4':				#mode 4 of gpgga, represents "Fixed"
			mode = "[3]" + "Fixed"					#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '5':				#mode 5 of gpgga, represents "Floating"
			mode = "[3]" + "Floating"				#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '6':				#mode 6 of gpgga, represents "Dead reckoning"
			mode = "[3]" + "Dead reckoning"			#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '7':				#mode 7 of gpgga, represents "Manual input"
			mode = "[3]" + "Manual input"			#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '8':				#mode 8 of gpgga, represents "Simulator"
			mode = "[3]" + "Simulator"				#setting up data to be send with FIFO
		elif (data['gpgga'][6]) is '9':				#mode 9 of gpgga, represents "WAAS"
			mode = "[3]" + "WAAS"					#setting up data to be send with FIFO
		else:										#when no mode is noticed dont write out anything
			mode = "[3]" + ". . ."					#setting up data to be send with FIFO
		return mode
	except Exception, e:							#error message handling when above try fails
		print (str(e))								#write out error message to terminal
                #print "Ik kom hier 3"
	return

#create subprocess for fifo
pipeIn, pipeOut = os.pipe()
try:
    pid = os.fork()
except Exception as e:
    print str(e)
    exit()
if pid is 0:
    #child process
    #os.close(pipeOut)
    fifoPort(pipeIn)
    exit()

#os.close(pipeIn)

#time.sleep(20)				#used to avoid startup interferance whit pi boot sequence
GPIO.setmode(GPIO.BCM)			#set gpio mode to enable control
GPIO.setup(0, GPIO.OUT)			#set pin 0 to output
GPIO.output(0, GPIO.HIGH)		#make pin 0 high
#portTry()				#call on function portTry
#scanPorts()				#call on function scanPorts
#port.close()
# Try to find novatel USB
port = getNRCPort()
if port is None:
        exit()
print port
while True:				#while loop to make the program run indefinitally
	#port = portDefine()			#call on function portDefine (TODO better description)
	serialData = readSerial(port)
        dataManager(serialData, pipeOut)
