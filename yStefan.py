#Written by Stefan van Delft 26/07/2017
#Display is added by Martijn Rombouts
#
#
#


#mattie regex
regexIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"			#this is used to filter out the IP adress


import serial                   #import the serial communication functions
import time			#time delay function imported
import datetime
import serial.tools.list_ports
import usb.core
import usb.util
import re
import sys
import commands
import RPi.GPIO as GPIO


def portTry():
	dev = usb.core.find(idVendor=0x09d7, idProduct=0x0100)

	if dev is None:
		#raise ValueError('Device not found')
		return

	elif dev is True:
		print("Device found")
		return

def portDefine():						#function to define the port the OEM7 is connected to
	try:							#testing for OEM7
		PORT = 		"/dev/ttyUSB1"
		port = serial.Serial('/dev/ttyUSB1', 9600)	#defining the serial port as a contant value
	except Exception, e:					#used to write out error
		#print error
		filewrite(str(e)+"\n")				#write out error to textdocument
		port = 0
		commands.wrt_str("Trying to connect",5)		#send an error message to the display
		print('\nUSB niet aangesloten\n')		#send an error message to the terminal


	readSerial(port)


def scanPorts():
	ports = list(serial.tools.list_ports.comports())
	for p in ports:
    		print p


def filewrite(rcv):                             		#Function to write data to a .txt file
	logfile = open("templog.txt", "a")      		#open file
	logfile.write(str(datetime.datetime.now()) + "\n")	#adding time of error
	logfile.write(rcv)                      		#write line in file
	logfile.close                           		#close file


def readSerial(port):						#reading all the data that is send by the OEM7
#Read serial
	try:							#testing if data is transmitted
		data = {'ip': None, 'gpgga': None, 'ins_active': None, 'ins_inactive': None, 'ins_aligning': None, 'ins_high_variance': None, 'ins_solution_good': None, 'ins_solution_free': None, 'ins_alignment_complete': None, 'determining_orientation': None, 'waiting_initialpos': None, 'waiting_azimuth': None, 'initializing_biases': None, 'motion_detect': None, 'finesteering': None, 'coarsesteering': None, 'unknown': None, 'aproximate': None, 'coarseadjusting': None, 'coarse': None, 'freewheeling': None, 'fineadjusting': None, 'fine': None, 'finebackupsteering': None, 'sattime': None, 'gpgga': None, 'ins': None}		#define what to expect in the dictionary
		j = 0
		rcv = [None]*10
		for x in range (0, 10):
			rcv[j] = port.readline()                 #rvc is the serial data received
			j = j + 1
		print("----------------------------------------\n")					#adding a line in the terminal for transparity
		print("----------------------------------------\n")					#adding a line in the terminal for transparity

		str1 = ''.join(rcv)
		for word in str1.split():
			m = re.search(regexIP, str1)		#let the regex filter out the ip of the text that was send
			if(m is not None and data['ip'] is None):
				data['ip'] = m.group()		#adding IP to the dictionary
			if(exact_Match(word,"FINESTEERING") and data['finesteering'] is None):		#
				data['finesteering'] = True						#adding finesteering to the dictionary
			if(exact_Match(word,"COARSESTEERING") and data['coarsesteering'] is None):
				data['coursesteering'] = True						#adding coursesteering to the dictonary
			if(exact_Match(word,"UNKNOWN") and data['unknown'] is None):		#
				data['unknown'] = True
			if(exact_Match(word,"APROXIMATE") and data['aproximate'] is None):		#
				data['aproximate'] = True
			if(exact_Match(word,"COARSEADJUSTING") and data['coarseadjusting'] is None):		#
				data['coarseadjusting'] = True
			if(exact_Match(word,"COARSE") and data['coarse'] is None):		#
				data['coarse'] = True
			if(exact_Match(word,"FREEWHEELING") and data['freewheeling'] is None):		#
				data['freewheeling'] = True
			if(exact_Match(word,"FINEADJUSTING") and data['fineadjusting'] is None):		#
				data['fineadjusting'] = True
			if(exact_Match(word,"FINE") and data['fine'] is None):		#
				data['fine'] = True
			if(exact_Match(word,"FINEBACKUPSTEERING") and data['finebackupsteering'] is None):		#
				data['finebackupsteering'] = True
			if(exact_Match(word,"SATTIME") and data['sattime'] is None):		#
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
		exportData(data)									#call exportData def
		displayData(data)									#call displayData def
		statusGPGGA(data)									#call statusGPGGA def
		port.close()										#close port to OEM7
		time.sleep(2)										#add a delay of 2 seconds


	except Exception, e:					#not receiving data from OEM7
		#print error
		filewrite(str(e)+"\n")				#write out error to text file
		port = 0					#define port as 0
		commands.wrt_str("Connection Error",2)		#write an error message to display
		print('\nUSB kan niet uitgelezen worden\n')	#write an error message to terminal
		time.sleep(10)					#put 10 second delay in before repeating try functions



def exportData(data):						#def that prints data to the terminal, used to check for problems
	print(data['gpgga'][6])					#print dictionary adress 6 in gpgga subclass
	print(data['gpgga'][7])					#print dictionary adress 7 in gpgga subclass
	print(data['ip'])					#print dictionary ip
	print(data['finesteering'])				#print dictionary finesteering
	tryIns(data)						#call on tryIns function
	return

def displayData(data):						#main write out to the display

	#Sattalites
	commands.wrt_str(data['gpgga'][7],7)			#write out the amount of sattalites are in contact with OEM7
	IP_String = bytearray()
	#IP_String.extend(" ")
	IP_String.extend(data['ip'])
	commands.wrt_str(IP_String,1)				#write out the IP adress to the first string adress on the display
	#commands.wrt_str(data['ip'])

	if (data['finesteering'] == True):			#testing for finesteering
		commands.wrt_str("Fine steering",5)		#write out 'Fine' to the 5th string adress on the display
	elif (data['coarsesteering'] == True):			#testing for coursesteering
		commands.wrt_str("Coarse steering",5)			#write out 'Course' to the 5th string adress on the display
    	elif (data['unknown'] == True):
		commands.wrt_str("Unknown",5)
    	elif (data['aprocimate'] == True):
		commands.wrt_str("Aproximate",5)
    	elif (data['coarseadjusting'] == True):
		commands.wrt_str("Coarse adjusting",5)
    	elif (data['coarse'] == True):
		commands.wrt_str("Coarse",5)
    	elif (data['freewheeling'] == True):
		commands.wrt_str("Freewheeling",5)
    	elif (data['fineadjusting'] == True):
		commands.wrt_str("Fineadjusting",5)
    	elif (data['Fine'] == True):
		commands.wrt_str("Fine",5)
    	elif (data['finebackupsteering'] == True):
		commands.wrt_str("Fine backupsteering",5)
    	elif (data['sattime'] == True):
		commands.wrt_str("sattime",5)

	#tryIns(data)
	return


def tryIns(data):							#def to determine INS
	try:								#try to define, if failed goes to except
		partup = (data['ins'][20])				#define dictionary entry 20 from ins as partup for further filtering
		clean_Ins = partup.split('*')				#split up partup
		data['insclean'] = clean_Ins				#add the split entry's as seperate dictionary adresses
		print(data['insclean'][0])				#print the wanted dictionary adress to the terminal for control
		if (data['ins_active'] == True):		#check INS if it is active
			commands.wrt_str("Ins active",2)		#write to display on adress 2 of the string list
		elif (data['ins_aligning'] == True):
			commands.wrt_str("Ins aligning",2)
		elif (data['ins_high_variance'] == True):
			commands.wrt_str("Ins high variance",2)
		elif (data['ins_solution_good'] == True):
			commands.wrt_str("Ins solution good",2)
		elif (data['ins_solution_free'] == True):
			commands.wrt_str("Ins solution free",2)
		elif (data['ins_alignment_complete'] == True):
			commands.wrt_str("Ins alignment complete",2)
		elif (data['determining_orientation'] == True):
			commands.wrt_str("Determining orientation",2)
		elif (data['waiting_initialpos'] == True):
			commands.wrt_str("Waiting initialpos",2)
		elif (data['waiting_azimuth'] == True):
			commands.wrt_str("Waiting azimuth",2)
		elif (data['initializing_biases'] == True):
			commands.wrt_str("Initializing biases",2)
		elif (data['motion_detect'] == True):
			commands.wrt_str("Motion detect",2)
		else:							#when INS is inactive
			commands.wrt_str("Ins inactive",2)		#write to display on adress 2 of the string list
			return
	except Exception, e:						#error handling INS testing
		#print error
		filewrite(str(e)+"\n")					#write error to text file
		print (str(e))						#write error to the terminal


def findWord(phrase, word):						#word seacher that is used by readSerial
	if(phrase.find(word) > 0):					#testing for an exact match
		return True						#return true to confirm the word
	return False


def exact_Match(phrase, word):						#exact match def for filtering words
    b = r'(\s|^|$)'
    res = re.match(b + word + b, phrase, flags=re.IGNORECASE)
    return bool(res)


def statusGPGGA(data):							#used to determine the status for GPGGA
	try:								#determine the gpgga status
		if (data['gpgga'][6]) is '1':				#mode 1
			print "non"					#
		elif (data['gpgga'][6]) is '2':				#mode 2
			print "non2"					#
		elif (data['gpgga'][6]) is '3':				#mode 3
			print "non3"					#
		elif (data['gpgga'][6]) is '4':				#mode 4
			print "fixxed"					#fixxed position
		elif (data['gpgga'][6]) is '5':				#mode 5
			print "float"					#float mode
		elif (data['gpgga'][6]) is '6':				#mode 6
			print "waas"					#waas mode
		elif (data['gpgga'][6]) is '7':				#mode 7
			print "non7"					#
		elif (data['gpgga'][6]) is '8':				#mode 8
			print "non8"					#
		elif (data['gpgga'][6]) is '9':				#mode 9
			print "basestation"				#serving as basestation
		elif (data['gpgga'][6]) is '10':			#mode 10
			print "non10"					#
		else:							#when no mode is noticed dont write out anything
			print "niets"					#
	except Exception, e:						#error message handling when above try fails
		print (str(e))						#write out error message to terminal
	return


time.sleep(20)				#used to avoid startup interferance whit pi boot sequence
GPIO.setmode(GPIO.BCM)			#set gpio mode to enable control
GPIO.setup(0, GPIO.OUT)			#set pin 0 to output
GPIO.output(0, GPIO.HIGH)		#make pin 0 high
portTry()				#call on function portTry
scanPorts()				#call on function scanPorts
#port.close()
while True:				#while loop to make the program run indefinitally
	portDefine()			#call on function portDefine
