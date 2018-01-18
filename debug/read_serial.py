import serial
ser = serial.Serial('/dev/ttyAMA0', timeout=1)  # open serial port
data = None
while True:
	# data = ser.readline()     # write a string
	data = ser.read(100)     # write a string
	if data is not 0:
		print len(data)
		print data
		data = None
ser.close()             # close port
