import json
import requests

#fonction permettant de remplacer les données incohérentes (-128) par des valeurs null
def formaterDonnees(lecture, donneesLues):
    donnees = donneesLues[(donneesLues.index(lecture))].split(';')
    donnees = map(int, donnees)
    donnees = list(map(int,donnees))    
    #donnees = [None if x==-128 else x for x in donnees]
    return donnees

#fonction permettant de placer les données dans le format standard pour l'envoi à l'api
def formerJson(donneesLues):
    for lecture in donneesLues:
        #données qui seront envoyées à l'API
        donneesJson = {}
        #valeur associée à la clé data dans le dictionnaire donneesJson
        valeursCapteurs = []
        #tableau avec les données d'une lecture
        donnees = formaterDonnees(lecture, donneesLues)

        cptValeursCapteurs = 0
        cptDonnees = 0
        
        while cptDonnees < len(donnees) - 1:
            valeursCapteurs.append( { "id" : donnees[cptDonnees], "L1" : donnees[cptDonnees+1], "L2" : donnees[cptDonnees+2], "L3" : donnees[cptDonnees+3] } )
            cptValeursCapteurs += 1
            cptDonnees += 4

        if len(valeursCapteurs) > 0:
            donneesJson.update( { "data" : valeursCapteurs } )
            url = 'http://206.167.36.216/api/lecture_data/'
            payload = json.dumps(donneesJson)                                                                                                           
            headers = {'Content-Type': 'application/json'}
            response = requests.post(url, payload, headers=headers)
            print(payload)
            print(response.status_code)

