import os
import RPi.GPIO as GPIO

from werserver.seabed-server.web.services.dataManager import dataManager

# Create fifo to communicate with genieInterface.c
# Filter data
class GenieInterface(object):
    FIFO = '/tmp/mypipe'
    BUFFSIZE = 1024
    REGEXIP = r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}"			

    def __init__(self):
        # set gpio mode to enable control
        GPIO.setmode(GPIO.BCM)			
        # set pin 0 to output
        GPIO.setup(0, GPIO.OUT)			
        # make pin 0 high
        GPIO.output(0, GPIO.HIGH)		
        createFifo(self)

    # init fifo when initialised
    def createFifo(self):
        try:
            os.mkfifo(self.FIFO)
        except OSError as oe:
            # logger.error(oe)
            if oe.errno != errno.EEXIST:
                raise

    # write data to fifo
    def write(self, data):
        try:
            fifo = os.open(self.FIFO, os.O_WRONLY)
            # data =  os.read(pipeIn, BUFFSIZE)
            os.write(fifo, data + '\n')
            fifo.close()
        except Exception as e:
            print str(e)


    def __exact_Match(phrase, word):
        b = r'(\s|^|$)'
        res = re.match(b + word + b, phrase, flags=re.IGNORECASE)
        return bool(res)



    # Use this command to get data
    def getData():
        data = [None]*6
        raw = __filterRaw(self, data)
        data[0] = raw['ip']
        data[1] = __filterStatus(raw)
        data[2] = __filterHeading(raw)
        data[3] = __filterIns(raw)
        data[4] = __filterGpgga(raw)
        data[5] = ("[7]" + raw['gpgga'][7])
        return data

#heading = 4
#satallite = 7
#rtk = 3
#ip = 0
#position = 1
#status = 2

    # return dictionary with filtered data
    def __filterRaw(self, data):

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
                'gphdt2': None, \
                'ins': None}

        str1 = ''.join(rcv)
        for word in str1.split():
            m = re.search(REGEXiP, str1)
            if(m is not None and data['ip'] is None):
                data['ip'] = "[0]" + m.group()
            if(exact_Match(word,"FINESTEERING") and data['finesteering'] is None):
                data['finesteering'] = True
            if(exact_Match(word,"COARSESTEERING") and data['coarsesteering'] is None):
                data['coursesteering'] = True
            if(exact_Match(word,"UNKNOWN") and data['unknown'] is None):	
                data['unknown'] = True
            if(exact_Match(word,"APROXIMATE") and data['aproximate'] is None):
                data['aproximate'] = True
            if(exact_Match(word,"COARSEADJUSTING") and data['coarseadjusting'] is None):
                data['coarseadjusting'] = True
            if(exact_Match(word,"COARSE") and data['coarse'] is None):
                data['coarse'] = True
            if(exact_Match(word,"FREEWHEELING") and data['freewheeling'] is None):
                data['freewheeling'] = True
            if(exact_Match(word,"FINEADJUSTING") and data['fineadjusting'] is None):
                data['fineadjusting'] = True
            if(exact_Match(word,"FINE") and data['fine'] is None):
                data['fine'] = True
            if(exact_Match(word,"FINEBACKUPSTEERING") and data['finebackupsteering'] is None):
                data['finebackupsteering'] = True
            if(exact_Match(word,"SATTIME") and data['sattime'] is None):
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
            if(findWord(word,"GPGGA") and data['gpgga'] is None):
                mylist = word.split(',')
                data['gpgga'] = mylist
            if(findWord(word,"INSPVAA") and data['ins'] is None):
                mylist2 = word.split(',')
                data['ins'] = mylist2
            if(findWord(word,"GPHDT,")and data['gphdt'] is None):
                data['gphdt'] = word.split(',')
                data['gphdt2'] = (data['gphdt'][0]).split('$')
                print data['gphdt2'][1]
        return data


    def __filterStatus(data):
        IP_String = bytearray()
        IP_String.extend(data['ip'])
        if (data['finesteering'] == True):
            mode = "[1]" + "Fine steering"
        elif (data['coarsesteering'] == True):
            mode = "[1]" + "Coarse steering"
        elif (data['unknown'] == True):
            mode = "[1]" + "Unknown"
        elif (data['aproximate'] == True):
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


    def __filterHeading(data):
            try:
                if ((data['gphdt2'][1]) == 'GPHDT'):
                    mode = "[4]" + "OK"
                else:
                    mode = "[4]" + "Non"
            except Exception as e:
                print str(e)
            return mode


    def __filterIns(data):
        mode = "[2]" +  "unknown"
        try:
            partup = (data['ins'][20])
            clean_Ins = partup.split('*')
            data['insclean'] = clean_Ins
            if(exact_Match((data['insclean'][0]),"INS_ACTIVE")):
                mode = "[2]" + "Ins active"
            elif (exact_Match((data['insclean'][0]),"INS_ALIGNING")):
                mode = "[2]" + "Ins aligning"
            elif (exact_Match((data['insclean'][0]),"INS_HIGH_VARIANCE")):
                mode = "[2]" + "Ins high variance"
            elif (exact_Match((data['insclean'][0]),"INS_SOLUTION_GOOD")):
                mode = "[2]" + "Ins solution good"
            elif (exact_Match((data['insclean'][0]),"INS_SOLUTION_FREE")):
                mode = "[2]" + "Ins solution free"
            elif (exact_Match((data['insclean'][0]),"INS_ALIGNMENT_COMPLETE")):
                mode = "[2]" + "Ins align complete" 	
            elif (exact_Match((data['insclean'][0]),"DETERMINING_ORIENTATION")):
                mode = "[2]" + "Determ orientation"
            elif (exact_Match((data['insclean'][0]),"WAITING_INITIALPOS")):
                mode = "[2]" + "Waiting initialpos"
            elif (exact_Match((data['insclean'][0]),"WAITING_AZIMUTH")):
                mode = "[2]" + "Waiting azimuth"
            elif (exact_Match((data['insclean'][0]),"INITIALIZING_BIASES")):
                mode = "[2]" + "Initializing biases"
            elif (exact_Match((data['insclean'][0]),"MOTION_DETECT")):
                mode = "[2]" + "Motion detect"
            elif (exact_Match((data['insclean'][0]),"INS_INACTIVE")):
                mode = "[2]" + "Ins inactive"
            return mode
        except Exception, e:
            filewrite(str(e)+"\n")
            print (str(e))
            print (data['ins_active'])
            return mode


# used to determine the status for GPGGA by reading a 
# number out of the input string. the defenition for 
# each number can be found in Novatel's manual.
    def __filterGPGGA(data):						
            try:
                    if (data['gpgga'][6]) is '0':
                            mode = "[3]" + "No fix"
                    elif (data['gpgga'][6]) is '1':
                            mode = "[3]" + "Single point"
                    elif (data['gpgga'][6]) is '2':
                            mode = "[3]" + "Pseudorange"
                    elif (data['gpgga'][6]) is '3':
                            mode = "[3]" + ". . ."
                    elif (data['gpgga'][6]) is '4':
                            mode = "[3]" + "Fixed"
                    elif (data['gpgga'][6]) is '5':
                            mode = "[3]" + "Floating"
                    elif (data['gpgga'][6]) is '6':
                            mode = "[3]" + "Dead reckoning"
                    elif (data['gpgga'][6]) is '7':
                            mode = "[3]" + "Manual input"
                    elif (data['gpgga'][6]) is '8':
                            mode = "[3]" + "Simulator"
                    elif (data['gpgga'][6]) is '9':
                            mode = "[3]" + "WAAS"
                    else:
                            mode = "[3]" + ". . ."
                    return mode
            except Exception, e:
                    print (str(e))
            return None



