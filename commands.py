#module for the display commands
#8/8/2017 write string function added

def wrt_str(strText,strNr):
	#strText= "abc"
	#strNr = 5

	import serial
	port = "/dev/ttyAMA0"
	baud = 9600

	port = serial.Serial(port, baud, timeout=3.0)

	#get length
	length = len(strText)
	#creating array to sent to display


	data = bytearray()
	#command write_str
	data.append(0x02)
	#string index
	data.append(strNr)
	#string length
	data.append(length)
	#Append the text
	data.extend(strText)
	#add the newline character
	#data.append('\n')

	checksum = 0
	for i in range(0,length + 3):
		checksum ^= data[i]

	data.append(checksum)
	port.write(data)
