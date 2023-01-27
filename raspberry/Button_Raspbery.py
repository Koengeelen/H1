

#$ sudo apt-get install python-rpi.gpio python3-rpi.gpio



import RPi.GPIO as GPIO 

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(10, GPIO.IN, pull_up=GPIO.PUD_DOWN)

while True:
    while GPIO.input(10) == GPIO.HIGH:
        #Code to read form the arduino and save a document as a csv
