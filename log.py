import logging
import sys

def getLogger():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)
    #logger.basicConfig(stream=sys.stderr, level=logging.DEBUG)

    # create a file handler
    handler = logging.FileHandler('python.log')
    handler.setLevel(logging.INFO)

    # create a logging format
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)

    # add the handlers to the logger
    logger.addHandler(handler)

    return logger

def setup_custom_logger(name):

    handler = logging.StreamHandler()

    handler = logging.FileHandler('todo.log')

    formatter = logging.Formatter(fmt='%(asctime)s - %(levelname)s - %(module)s - %(message)s')
    handler.setFormatter(formatter)
    handler.setLevel(logging.INFO)

    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)
    logger.addHandler(handler)
    return logger
