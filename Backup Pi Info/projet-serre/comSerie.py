import serial
import time
import RPi.GPIO as GPIO
import actionneur

#cette fonction permet d'etablir une communication serie entre le PAN et le RPI
#inspiree de la demonstation du site internet :
#https://www.instructables.com/id/Read-and-write-from-serial-port-with-Raspberry-Pi/
#fait par Marianne
def lireSerieV1():
    #il faut initialiser la liste avec des donnees sinon cela va lever une exception lorsqu'on va
    #associer une valeur a un index donne
    donneesLues = [1,2,3]
    index = 0
    #important de fixer le baudrate a 9600 car c'est celui du PAN
    #/dev/ttyUSB0 represente le nom du port USB du RPI (utiliser avec un USB-a (RPI) vers USB-b (PAN))
    try:
        port = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    except:
        print('erreur dans l`ouverture du port')
        
    while True:
        try:
            #temporaire, seulement utile pour donner le temps aux donnees
            #d'entrer lorsqu'on appuie sur le bouton du PAN
            if index == 0:
                print('Appuyez sur le bouton du PAN')
                time.sleep(10)
            ligne = port.readline().decode('utf-8')
            if len(ligne) > 0:
                donneesLues[index] = ligne
                print(donneesLues[index])
                index += 1
            else:
                #lorsque la communication serie est terminee, nous retournons les donnees collectees
                return donneesLues
        except:
            print('erreur dans la lecture serie')
            
def lireSerie():
    #important de fixer le baudrate a 9600 car c'est celui du PAN
    #/dev/ttyUSB0 represente le nom du port USB du RPI (utiliser avec un USB-a (RPI) vers USB-b (PAN))
    try:
        port = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    except:
        print('erreur dans l`ouverture du port')
        
    try:
        actionneur.emissionSignal(1, 23)
            
        ligne1 = str(port.readline())
        ligne2 = str(port.readline())
        ligne = ligne1 + ligne2
        print ('donnees brutes l1: ' + ligne1)
        print ('donnees brutes l2: ' + ligne2)
        print ('donnees brutes: ' + ligne)
        #retrait des caracteres 
        ligne = ligne.replace('b', '')
        ligne = ligne.replace("'", '')
        ligne = ligne.replace(' ', '')
        ligne = ligne.replace('\\r', '')
        ligne = ligne.replace('\\n', '')
        
        if len(ligne) > 1:
            donneesLues = ligne
            print('donnees filtrees: ' + ligne)
            return donneesLues
    except:
        print('erreur dans la lecture serie')