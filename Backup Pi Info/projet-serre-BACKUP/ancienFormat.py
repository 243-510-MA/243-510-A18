import json
import requests

#fonction permettant de remplacer les donnees incoherentes (-128) par des valeurs null
def formaterDonnees(donneesLues, lecture):
    donnees = donneesLues[(donneesLues.index(lecture))].split('; ')
    del donnees[0:5]
    donnees = map(int, donnees)
    donnees = list(map(int,donnees))
    #en commentaire car cela donne un 400 (API)
    #donnees = [None if x==-128 else x for x in donnees]
    return donnees

#fonction permettant d'envoyer les donnees transformees a l'api
def postLecture(donneesJson):                     
    url = 'http://206.167.36.216/api/lecture_pan/'
    payload = json.dumps(donneesJson)                                                                                                           
    headers = {'Content-Type': 'application/json'}
    response = requests.post(url, payload, headers=headers)
    print(response.status_code)

#fonction permettant de placer les donnees dans le format standard pour l'envoi a l'api
def formerJson(donneesLues):
    if len(donneesLues)>1:
        del donneesLues[0]

        for lecture in donneesLues:
            #donnees qui seront envoyees a l'API
            donneesJson = {}
            #valeur associee a la cle data dans le dictionnaire donneesJson
            valeursCapteurs = []
            #tableau avec les donnees d'une lecture
            donnees = formaterDonnees(donneesLues, lecture)

            cptValeursCapteurs = 1
            cptDonnees = 0
            
            while cptDonnees < len(donnees) - 1:
                valeursCapteurs.append( { "id" : cptValeursCapteurs, "L1" : donnees[cptDonnees], "L2" : donnees[cptDonnees+1], "L3" : donnees[cptDonnees+2] } )
                cptValeursCapteurs += 1
                cptDonnees += 3
            
            if len(valeursCapteurs) > 0:
                donneesJson.update( { "data" : valeursCapteurs } )
                postLecture(donneesJson)
            
#fonction permettant de faire un call api pour savoir si l'utilisateur veut une lecture instantanee            
def getLectureInstantanee():
    response = requests.get('http://206.167.36.216/api/lecture_actionneur/')
    data = response.json()
    return data['isRequested']    

#fonction permettant de faire un call api pour modifier la valeur du parametre une fois qu'on a envoye la lecture 
def putLectureInstantanee(valeur):
    url = 'http://206.167.36.216/api/lecture_actionneur/'
    payload = json.dumps({"isRequested": valeur})                                                                                                           
    headers = {'Content-Type': 'application/json'}
    response = requests.put(url, payload, headers=headers)
    return response.status_code