import RPi.GPIO as GPIO
import time

#cette fonction permet d'envoyer une impultion electrique a une pin du GPIO un certain nombre de fois
#inspiree de la demonstration du site internet :
#https://thepihut.com/blogs/raspberry-pi-tutorials/27968772-turning-on-an-led-with-your-raspberry-pis-gpio-pins
#fait par Marianne
def emissionSignal(frequence, noPin):
    for x in range(frequence):
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(noPin,GPIO.OUT)
        #emission du signal
        GPIO.output(noPin,GPIO.HIGH)
        time.sleep(1)
        #arret du signal
        GPIO.output(noPin,GPIO.LOW)