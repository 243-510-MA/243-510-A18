import json
import requests

#Ce fichier regroupe l'ensemble des requetes faites a l'API ainsi que la transformation de donnees
#brutes en format JSON
#La structure des call api est inspiree des deux sites internet suivants :
#https://www.dataquest.io/blog/python-api-tutorial/
#https://www.pythonforbeginners.com/requests/using-requests-in-python/

#fonction permettant de remplacer les donnees incoherentes (-128) par des valeurs null
#a utiliser avec le format actuel de donnees
#fait par Marianne
def formaterDonneesV1(donneesLues, lecture):
    #une lecture a le format suivant : "19; 2; 24; 3; 0; 94; 6; 10; 83; 3; -4; -2; 5; -128"
    #representant les valeurs suivantes : "Annee; Mois; Jour; Heure; Minute; humiditeCarte1; temperatureAirCarte1; temperatureSolCarte1; humiditeCarte2; temperatureAirCarte2; temperatureSolCarte2; humiditeCarte3; temperatureAirCarte3; temperatureSolCarte3"
    #creation d'une liste dans laquelle un index represente une valeur dans la lecture
    donnees = donneesLues[(donneesLues.index(lecture))].split('; ')
    #on supprime les index 0 a 5 dans la liste (valeurs associees a la date et a l'heure)
    #del donnees[0:5]
    #convertit l'ensemble des valeurs en int
    donnees = list(map(int,donnees))
    #en commentaire car cela donne, pour l'instant, lorsqu'on envoie des valeurs null, un 400 (API)
    #operateur ternaire remplacant les -128 par des None (null du langage python)
    #donnees = [None if x==-128 else x for x in donnees]
    #une lecture a maintenant le format suivant (liste): [94, 6, 10, 83, 3, -4, -2, 5, -128]
    return donnees

#fonction permettant de convertir les valeurs en int
#a utiliser avec le nouveau format de donnees
def formaterDonnees(donneesLues):
#   print('formaterDonnees(' + donneesLues + ')')
    donnees = donneesLues.split(';')
    del donnees[len(donnees)-1]
    donnees = list(map(int,donnees))    
    #donnees = [None if x==-128 else x for x in donnees]
    return donnees


#fonction permettant d'envoyer les donnees transformees a l'api
#fait par Kurt
def postLecture(donneesJson):
    try:
        url = 'http://206.167.36.216:8080/api/lecture_pan/'
        payload = json.dumps(donneesJson)
        if not (payload is None):
            print('postLecture: payload = ' + payload)
        headers = {'Content-Type': 'application/json'}
        response = requests.post(url, donneesJson, headers=headers)
        #nous retournons le code http associe a la reponse de l'api
        return response.status_code
    except:
        print('erreur post ' +  str(response.json()))

#fonction permettant de faire un call api pour savoir si l'utilisateur veut une lecture instantanee
#fait par Kurt
def getLectureInstantanee():
    try:
        response = requests.get('http://206.167.36.216:8080/api/lecture_actionneur/')
        #body de la reponse
        data = response.json()
        #valeur associee a la clef isRequested (API)
        return data['isRequested']    
    except:
        print('erreur getLecture ' + str(response.json()))
        
#fonction permettant de faire un call api pour modifier la valeur du parametre une fois qu'on a envoye
#la lecture
#fait par Kurt
def putLectureInstantanee(valeur):
    try:
        url = 'http://206.167.36.216:8080/api/lecture_actionneur/'
        payload = json.dumps({"isRequested": valeur})                                                                                                           
        headers = {'Content-Type': 'application/json'}
        response = requests.put(url, payload, headers=headers)
        #nous retournons le code http associe a la reponse de l'api
        return response.status_code
    except:
        print('erreur putLecture ' +  str(response.json()))

#fonction permettant de placer les donnees dans le format standard pour l'envoi a l'api
#cette fonction est utilisee pour le format actuel de donnees du PAN
#fait par Kurt et Marianne
def formerJsonV1(donneesLues):
    if len(donneesLues) > 1:
        del donneesLues[0]
        
        for lecture in donneesLues:
            #donnees qui seront envoyees a l'API
            donneesJson = {}
            #valeur associee a la cle data dans le dictionnaire donneesJson
            valeursCapteurs = []
            #tableau avec les donnees d'une lecture
            donnees = formaterDonnees(donneesLues, lecture)

            cptValeursCapteurs = 1
            cptDonnees = 5
            
            while cptDonnees < len(donnees) - 1:
                #formation du json du tableau pour avoir le format standard soit :
                #[{ "id" : idCarte1, "L1": valeurLecture1, "L2" : valeurLecture2, "L3" : valeurLecture3 },
                # { "id" : idCarte2, "L1": valeurLecture1, "L2" : valeurLecture2, "L3" : valeurLecture3 }]
                #ci-dessus le contenu du tableau valeursCapteurs
                valeursCapteurs.append( { "id" : cptValeursCapteurs, "L1" : donnees[cptDonnees], "L2" : donnees[cptDonnees+1], "L3" : donnees[cptDonnees+2] } )
                cptValeursCapteurs += 1
                #on traite les donnees en blocs de 3 valeurs
                cptDonnees += 3
            
            if len(valeursCapteurs) > 0:
                #on forme le dictionnaire donneesJson 
                donneesJson.update( { "data": { "annee": donnees[0] + 2000, "mois": donnees[1], "jour": donnees[2], "heure": donnees[3], "minute": donnees[4], "lectures": valeursCapteurs } } )
                return postLecture(donneesJson)
                
#fonction permettant de placer les donnees dans le format standard pour l'envoi a l'api
#cette fonction est utilisee pour le nouveau format de donnees du PAN
#fait par Kurt
def formerJson(donneesLues):
    #donnees qui seront envoyees a l'API
    donneesJson = {}
    #valeur associee a la cle data dans le dictionnaire donneesJson
    valeursCapteurs = []
    #tableau avec les donnees d'une lecture
    donnees = formaterDonnees(donneesLues)

    cptValeursCapteurs = 0
    cptDonnees = 0
    
    valeurs = "["
    
    while cptDonnees < len(donnees) - 1:
        valeurs = valeurs + '{"id":'+str(donnees[cptDonnees])+',"L1":'+str(donnees[cptDonnees+1])+',"L2":'+str(donnees[cptDonnees+2])+',"L3":'+str(donnees[cptDonnees+3])+'}'
#       valeursCapteurs.append( { "id" : donnees[cptDonnees], "L1" : donnees[cptDonnees+1], "L2" : donnees[cptDonnees+2], "L3" : donnees[cptDonnees+3] } )
#        cptValeursCapteurs += 1
        valeurs = valeurs + ","
        cptDonnees += 4

#    if len(valeursCapteurs) > 0:
    if len(valeurs) > 1:
#        donneesJson.update( { "data" : valeursCapteurs } )
        valeurs = valeurs.rstrip(',') + ']';
        valeurs = '{"data":' + valeurs + '}'
#        return donneesJson
        return valeurs