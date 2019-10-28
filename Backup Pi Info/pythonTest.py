import json

data = {}
data['people'] = []
data['people'].append({
    'name': 'Marianne',
    'age': '22'
})
data['people'].append({
    'name': 'Bob',
    'age': '44'
})
data['people'].append({
    'name': 'Builder',
    'age': '56'
})

with open('data.txt', 'w') as outfile:
    json.dump(data, outfile)


with open('data.txt', 'r') as json_file:
    data = json.load(json_file)
    for p in data['people']:
       print('Name: ' + p['name'])
       print('Age: ' + p['age'])
       print('')