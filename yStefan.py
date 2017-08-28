#Written by Stefan van Delft 26/07/2017
#Display is added by Martijn Rombouts
#
#Quick Data logger. 
#
#This program looks for a serial line on ttyUSB4
#When it receives serial data it opens a txt file, writes the line in it
#then closes the file again. 
#
#  log version
#  log ipconfig
#  log gpgga
#  log inspvaa


#mattie regex
regexIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"			#this is used to filter out the IP adress


#Misschien andere naam
#PORT = 		"/dev/ttyUSB1"
		

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

#port = serial.Serial('/dev/ttyUSB1', 9600)
#port = serial.Serial("/dev/ttyUSB1", baudrate=9600, timeout=3.0, rtscts=1, dsrdtr=1, xonxoff=1)     #serial port settings
#port.close()


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


def readSerial(port):						#reading al the data that is send by the OEM7
#Read serial
	try:							#testing if data is transmitted
		data = {'ip': None, 'gpgga': None, 'finesteering': None, 'coursesteering': None, 'gpgga': None, 'ins': None}		#define what to expect in the dictionary
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
				data['coursesteering'] = None						#removing coursesteering from dictionary
			if(exact_Match(word,"COURSESTEERING") and data['COURSESTEERING'] is None):
				data['coursesteering'] = True						#adding coursesteering to the dictonary
				data['finesteering'] = None						#removing finesteering from the dictionary
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
		commands.wrt_str("Fine",5)			#write out 'Fine' to the 5th string adress on the display
	elif (data['coursesteering'] == True):			#testing for coursesteering
		commands.wrt_str("Course",5)			#write out 'Course' to the 5th string adress on the display
	#tryIns(data)
	return


def tryIns(data):							#def to determine INS
	try:								#try to define, if failed goes to except
		partup = (data['ins'][20])				#define dictionary entry 20 from ins as partup for further filtering
		clean_Ins = partup.split('*')				#split up partup
		data['insclean'] = clean_Ins				#add the split entry's as seperate dictionary adresses
		print(data['insclean'][0])				#print the wanted dictionary adress to the terminal for control
		if (data['insclean'][0] == "INS_ACTIVE"):		#check INS if it is active
			commands.wrt_str("Ins active",2)		#write to display on adress 2 of the string list
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
	try:								#try
		if (data['gpgga'][6]) is '1':
			print "non"
		elif (data['gpgga'][6]) is '2':
			print "non2"
		elif (data['gpgga'][6]) is '3':
			print "non3"
		elif (data['gpgga'][6]) is '4':
			print "fixxed"
		elif (data['gpgga'][6]) is '5':
			print "float"
		elif (data['gpgga'][6]) is '6':
			print "waas"
		elif (data['gpgga'][6]) is '7':
			print "non7"
		elif (data['gpgga'][6]) is '8':
			print "non8"
		elif (data['gpgga'][6]) is '9':
			print "basestation"
		elif (data['gpgga'][6]) is '10':
			print "non10"
		else:
			print "niets"
	except Exception, e:
		print (str(e))
		print('gefaald')
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
