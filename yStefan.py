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

COMMAND1 = 		'log version ontime 1'.encode('utf-8')
COMMAND2 = 		"log ipconfig ontime 1"
COMMAND3 = 		"log gpgga ontime 1"
COMMAND4 = 		"log inspvaa ontime 1"

#mattie regex
regexIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"


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

def portDefine():
	try:
		PORT = 		"/dev/ttyUSB1"
		port = serial.Serial('/dev/ttyUSB1', 9600)
	except Exception, e:
		#print error
		filewrite(str(e)+"\n")
		port = 0
		#print (str(e))
		print('\nUSB niet aangesloten\n')


	readSerial(port)


def scanPorts():
	ports = list(serial.tools.list_ports.comports())
	for p in ports:
    		print p


def filewrite(rcv):                             #Function to write data to a .txt file
	logfile = open("templog.txt", "a")      #open file
	logfile.write(str(datetime.datetime.now()) + "\n")
	logfile.write(rcv)                      #write line in file
	logfile.close                           #close file


def readSerial(port):
#Read serial
	try:	
		data = {'ip': None, 'gpgga': None, 'finesteering': None, 'coursesteering': None, 'gpgga': None, 'ins': None}
		j = 0
		rcv = [None]*10
		for x in range (0, 10):
			rcv[j] = port.readline()                  #rvc is the 	serial data received
			j = j + 1
		print("----------------------------------------\n")
		print("----------------------------------------\n")

		str1 = ''.join(rcv)
		for word in str1.split():
			m = re.search(regexIP, str1)
			if(m is not None and data['ip'] is None):
				data['ip'] = m.group()
			if(exact_Match(word,"FINESTEERING") and data['finesteering'] is None):
				data['finesteering'] = True
				data['coursesteering'] = None
			if(exact_Match(word,"COURSESTEERING") and data['COURSESTEERING'] is None):
				data['coursesteering'] = True
				data['finesteering'] = None
			if(findWord(word,"GPGGA") and data['gpgga'] is None):
				string = word
				mylist = string.split(',')
				data['gpgga'] = mylist
			if(findWord(word,"INS_") and data['ins'] is None):
				string2 = word
				mylist2 = string2.split(',')
				data['ins'] = mylist2	
		exportData(data)
		displayData(data)
		statusGPGGA(data)


	except Exception, e:
		#print error
		filewrite(str(e)+"\n")
		port = 0
		#print (str(e))
		print('\nUSB kan niet uitgelezen worden\n')
		time.sleep(10)
	


def exportData(data):
	print(data['gpgga'][6])
	print(data['gpgga'][7])
	print(data['ip'])
	print(data['finesteering'])
	tryIns(data)
	return

def displayData(data):

	#Sattalites
	commands.wrt_str(data['gpgga'][7],7)
	IP_String = bytearray()
	IP_String.extend(" ")
	IP_String.extend(data['ip'])
	commands.wrt_str(IP_String,1)

	if (data['finesteering'] == True):
		commands.wrt_str("OK",5)
	elif (data['coursesteering'] == True):
		commands.wrt_str("OK",5)
	#tryIns(data)
	return


def tryIns(data):
	try:
		partup = (data['ins'][20])
		clean_Ins = partup.split('*')
		data['insclean'] = clean_Ins
		print(data['insclean'][0])
		if (data['insclean'][0] == "INS_ACTIVE"):
			commands.wrt_str("Ins active",2)
		else:
			commands.wrt_str("Ins inactive",2)
			#commands.wrt_str("N.A.",4)
			return
	except Exception, e:
		#print error
		filewrite(str(e)+"\n")
		print (str(e))
		print('gefaald')


def findWord(phrase, word):
	if(phrase.find(word) > 0):
		return True
	return False


def exact_Match(phrase, word):
    b = r'(\s|^|$)' 
    res = re.match(b + word + b, phrase, flags=re.IGNORECASE)
    return bool(res)


def statusGPGGA(data):
	try:
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


def getGPGAPP():
#Get GPGAPP from data
	pass
	return GPGAPP


def requestData():
#gets the data from the Novatel receiver by forwarding four commands over USB
	pass

time.sleep(20)
GPIO.setmode(GPIO.BCM)
GPIO.setup(0, GPIO.OUT)
GPIO.output(0, GPIO.HIGH)
portTry()
scanPorts()
#port.open()
while True:
	portDefine()
	#time.sleep(10)

