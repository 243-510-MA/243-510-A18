#!/usr/bin/python2

import comApi
import comSerie
import actionneur
import time

#Les deux listes suivantes sont necessaires si on a besoin de faire des tests sans la
#lecture en serie (sans le PAN). Les donnees simulent ce que le PAN envoie au RPI.
#format de donnees actuel
#decommenter pour tester sans le PAN
'''
donneesLues = [
    "Annee; Mois; Jour; Heure; Minute; humiditeCarte1; temperatureAirCarte1; temperatureSolCarte1; humiditeCarte2; temperatureAirCarte2; temperatureSolCarte2; humiditeCarte3; temperatureAirCarte3; temperatureSolCarte3",
    "19; 2; 24; 3; 0; 94; 6; 10; 83; 3; -4; -2; 5; -128",
    "19; 2; 25; 4; 0; 95; 7; 11; 84; 4; -3; -1; 6; -128",
    "19; 2; 26; 5; 0; 96; 8; 12; 85; 5; -2; 0; 7; -128",
    "19; 2; 27; 6; 0; 97; 9; 13; 86; 6; -1; 1; 8; -128"
]
'''
#Nouveau format de donnees
'''
donneesLues = [
    "1;93;3;4;2;87;0;-7;3;-128;1;-128"
]
'''

#fonction chargee d'appeller les fonctions permettant de lire les donnees en serie,
#de les transformees en JSON et de les envoyees a l'API
def lireEtTraiterDonnees():
    #commenter la ligne suivante pour faire le test sans le PAN
    donneesLues = comSerie.lireSerie()
    print('Serial: ')
    print(donneesLues)
    while donneesLues is None:
        time.sleep(1)
        donneesLues = comSerie.lireSerie()
    return comApi.formerJson(donneesLues)

#fonction chargee d'appeller les fonctions permettant de lire les donnees en serie,
#de les transformees en JSON et de les envoyees a l'API
def lectureImmediateV1():
    #commenter la ligne suivante pour faire le test sans le PAN
    #donneesLues = comSerie.lireSerie()
    derniereLecture = len(donneesLues) - 1
    del donneesLues[1:derniereLecture]
    return comApi.formerJson(donneesLues)

#stockage de l'heure a laquelle le programme a ete execute
tempsDepart = time.time()

#Cette variable doit etre changee si vous voulez choisir l'intervalle des lectures.
#L'unite de temps utilisee est la seconde
frequenceLecture = 900

#boucle infinie qui s'execute aux 15 secondes 
while True:
    print('Sa roule')	#DEBUG
    #fait par Kurt
    #on verifie si l'utilisateur veut une lecture instantanee
    lectureInstantanee = comApi.getLectureInstantanee()
    #print(lectureInstantanee)
    if lectureInstantanee:
        donneesJson = lireEtTraiterDonnees()
        #print('Coucou: ' + donnesJson)
        if not (donneesJson is None):
            print ('message Json: ' + donneesJson)
            statut = comApi.postLecture(donneesJson)
            if statut == 201:
                #une fois le traitement termine, on remet le booleen a false
                comApi.putLectureInstantanee(False)
                #le temoin lumineux clignottera 1 fois pour confirmer l'envoie de la lecture
                actionneur.emissionSignal(1, 18)
                print('donnees instantanees envoyees a l`api')
            else:
                print('erreur dans l`envoi instantane ' + str(retour))
        
    #fait par Marianne
    tempsActuel = time.time()
    difference = tempsActuel - tempsDepart
     
    #si 1h s'est ecoulee, on fait une lecture  
    if difference >= frequenceLecture:
        #statut = lecture()
        donneesJson = lireEtTraiterDonnees()
        if not (donneesJson is None):
            print ('message Json: ' + donneesJson)
        statut = comApi.postLecture(donneesJson)
        if statut == 201:
            #le temoin lumineux clignottera 3 fois pour confirmer l'envoie de la lecture
            actionneur.emissionSignal(3, 18)
            print('donnees envoyees a l`api')
        else:
            print('erreur dans l`envoi ' + str(statut))
            
        #attribution d'une nouvelle valeur au tempsDepart
        tempsDepart = time.time()
    
    #aux 15 secondes, nous demandons a l'api si elle a besoin d'une lecture instantanee
    time.sleep(5)
