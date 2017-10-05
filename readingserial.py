def readSerial(port):						#reading all the data that is send by the OEM7
#Read serial
    import serial
    import time
    import commands
    import re
    import sys
    regexIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"	
	

    def exact_Match(phrase, word):							#exact match def for filtering words
	b = r'(\s|^|$)'
	res = re.match(b + word + b, phrase, flags=re.IGNORECASE)
	return bool(res)


    def findWord(phrase, word):							#word seacher that is used by readSerial
	if(phrase.find(word) > 0):						#testing for an exact match
		return True							#return true to confirm the word
	return False


    try:							#testing if data is transmitted
	tdata = {'ip': None, 'gpgga': None, 'gphdt': None, 'ins_active': None, 'ins_inactive': None, 'ins_aligning': None, 'ins_high_variance': None, 'ins_solution_good': None, 'ins_solution_free': None, 'ins_alignment_complete': None, 'determining_orientation': None, 'waiting_initialpos': None, 'waiting_azimuth': None, 'initializing_biases': None, 'motion_detect': None, 'finesteering': None, 'coarsesteering': None, 'unknown': None, 'aproximate': None, 'coarseadjusting': None, 'coarse': None, 'freewheeling': None, 'fineadjusting': None, 'fine': None, 'finebackupsteering': None, 'sattime': None, 'gpgga': None, 'ins': None}		#define what to expect in the dictionary
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
		if(m is not None and tdata['ip'] is None):
			tdata['ip'] = m.group()								#adding IP to the dictionary
		if(exact_Match(word,"FINESTEERING") and tdata['finesteering'] is None):			#
			tdata['finesteering'] = True							#adding finesteering to the dictionary
		if(exact_Match(word,"COARSESTEERING") and tdata['coarsesteering'] is None):
			tdata['coursesteering'] = True							#adding coursesteering to the dictonary
		if(exact_Match(word,"UNKNOWN") and tdata['unknown'] is None):				#
			tdata['unknown'] = True
		if(exact_Match(word,"APROXIMATE") and tdata['aproximate'] is None):			#
			tdata['aproximate'] = True
		if(exact_Match(word,"COARSEADJUSTING") and tdata['coarseadjusting'] is None):		#
			tdata['coarseadjusting'] = True
		if(exact_Match(word,"COARSE") and tdata['coarse'] is None):				#
			tdata['coarse'] = True
		if(exact_Match(word,"FREEWHEELING") and tdata['freewheeling'] is None):			#
			tdata['freewheeling'] = True
		if(exact_Match(word,"FINEADJUSTING") and tdata['fineadjusting'] is None):		#
			tdata['fineadjusting'] = True
		if(exact_Match(word,"FINE") and tdata['fine'] is None):					#
			tdata['fine'] = True
		if(exact_Match(word,"FINEBACKUPSTEERING") and tdata['finebackupsteering'] is None):	#
			tdata['finebackupsteering'] = True
		if(exact_Match(word,"SATTIME") and tdata['sattime'] is None):				#
			tdata['sattime'] = True
       		if(exact_Match(word,"INS_ACTIVE") and tdata['ins_active'] is None):
       			tdata['ins_active'] = True
       		if(exact_Match(word,"INS_INACTIVE") and tdata['ins_inactive']is None):
              		tdata['ins_inactive'] = True
       		if(exact_Match(word,"INS_ALIGNING") and tdata['ins_aligning'] is None):
               		tdata['ins_aligning'] = True
       		if(exact_Match(word,"INS_HIGH_VARIANCE") and tdata['ins_high_variance'] is None):
               		tdata['ins_high_variance'] = True
       		if(exact_Match(word,"INS_SOLUTION_GOOD") and tdata['ins_solution_good'] is None):
               		tdata['ins_solution_good'] = True
       		if(exact_Match(word,"INS_SOLUTION_FREE") and tdata['ins_solution_free'] is None):
               		tdata['ins_solution_free'] = True
       		if(exact_Match(word,"INS_ALIGNMENT_COMPLETE") and tdata['ins_alignment_complete'] is None):
               		tdata['ins_alignment_complete'] = True
       		if(exact_Match(word,"DETERMINING_ORIENTATION") and tdata['determining_orientation'] is None):
               		tdata['determining_orientation'] = True
       		if(exact_Match(word,"WAITING_INITIALPOS") and tdata['waiting_inititalpos'] is None):
               		tdata['waiting_initialpos'] = True
       		if(exact_Match(word,"WAITING_AZIMUTH") and tdata['waiting_azimuth'] is None):
               		tdata['waiting_azimuth'] = True
       		if(exact_Match(word,"INITIALIZING_BIASES") and tdata['initializing_biases'] is None):
               		tdata['initializing_biases'] = True
       		if(exact_Match(word,"MOTION_DETECT") and tdata['motion_detect'] is None):
       			tdata['motion_detect'] = True
		if(findWord(word,"GPHDT") and tdata['gphdt'] is None):
			#print("test123546")
			tdata['gphdt'] = True
		if(findWord(word,"GPGGA") and tdata['gpgga'] is None):				#getting GPGGA out of the read values
			mylist = word.split(',')						#split up the line in which GPGGA was found
			tdata['gpgga'] = mylist							#add GPGGA to the dictionary
		if(findWord(word,"INS_") and tdata['ins'] is None):				#getting INS out of the read values
			mylist2 = word.split(',')						#split up the line in which INS was found
			tdata['ins'] = mylist2							#add INs to the dictionary
		print tdata
        	return(tdata)
	print tdata
	return(tdata)


    except Exception, e:					#not receiving data from OEM7
	#print error
	#filewrite(str(e)+"\n")				#write out error to text file
	port = 0					#define port as 0
	commands.wrt_str("Connection Error",2)		#write an error message to display
	print('\nUSB kan niet uitgelezen worden\n')	#write an error message to terminal
	return(port)

    print"lmayo"
    return(tdata)
