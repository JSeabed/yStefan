def readSerial(port):						#reading all the data that is send by the OEM7
#Read serial
    import serial
    import time
    import commands
    
    try:							#testing if data is transmitted
	data = {'ip': None, 'gpgga': None, 'gphdt': None, 'ins_active': None, 'ins_inactive': None, 'ins_aligning': None, 'ins_high_variance': None, 'ins_solution_good': None, 'ins_solution_free': None, 'ins_alignment_complete': None, 'determining_orientation': None, 'waiting_initialpos': None, 'waiting_azimuth': None, 'initializing_biases': None, 'motion_detect': None, 'finesteering': None, 'coarsesteering': None, 'unknown': None, 'aproximate': None, 'coarseadjusting': None, 'coarse': None, 'freewheeling': None, 'fineadjusting': None, 'fine': None, 'finebackupsteering': None, 'sattime': None, 'gpgga': None, 'ins': None}		#define what to expect in the dictionary
	j = 0
	rcv = [None]*25
	for x in range (0, 25):
		rcv[j] = port.readline()                #rvc is the serial data received
		j = j + 1
	print("----------------------------------------\n")					#adding a line in the terminal for transparity
	print("----------------------------------------\n")					#adding a line in the terminal for transparity
	#print rcv
	str1 = ''.join(rcv)
	for word in str1.split():
		m = re.search(regexIP, str1)								#let the regex filter out the ip of the text that was send
		if(m is not None and data['ip'] is None):
			data['ip'] = m.group()								#adding IP to the dictionary
		if(exact_Match(word,"FINESTEERING") and data['finesteering'] is None):			#
			data['finesteering'] = True							#adding finesteering to the dictionary
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
		if(findWord(word,"GPHDT") and data['gphdt'] is None):
			#print("test123546")
			data['gphdt'] = True
		if(findWord(word,"GPGGA") and data['gpgga'] is None):				#getting GPGGA out of the read values
			mylist = word.split(',')						#split up the line in which GPGGA was found
			data['gpgga'] = mylist							#add GPGGA to the dictionary
		if(findWord(word,"INS_") and data['ins'] is None):				#getting INS out of the read values
			mylist2 = word.split(',')						#split up the line in which INS was found
			data['ins'] = mylist2							#add INs to the dictionary
        return(data)
    except Exception, e:					#not receiving data from OEM7
	#print error
	filewrite(str(e)+"\n")				#write out error to text file
	port = 0					#define port as 0
	commands.wrt_str("Connection Error",2)		#write an error message to display
	print('\nUSB kan niet uitgelezen worden\n')	#write an error message to terminal

