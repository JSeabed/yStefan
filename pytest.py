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
        fifo.write("test\n")
        fifo.write("123\n")
        fifo.write("456\n")
        fifo.close()
        #while True:
        #    data = fifo.read()
        #    if len(data) == 0:
        #            print("Writer closed")
        #            break
        #    print('Read: "{0}"'.format(data))
