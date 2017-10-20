import os
import errno

FIFO = '/tmp/mypipe'

try:
    os.mkfifo(FIFO)
except OSError as oe: 
    if oe.errno != errno.EEXIST:
        raise

    print("Opening FIFO...")
    with open(FIFO, "w", 1) as fifo:
        print("FIFO opened")
        fifo.write("test")
        fifo.write("123")
        fifo.write("456")
        fifo.close()
        #while True:
        #    data = fifo.read()
        #    if len(data) == 0:
        #            print("Writer closed")
        #            break
        #    print('Read: "{0}"'.format(data))
