import json

class JsonFile ():
    def __init__ (self, file_name):
        self.file_name = file_name
        file = open(self.file_name, 'r')
        self.objects = json.loads(file.read())
        file.close()

    def write_changes (self):
        data = json.dumps(self.objects)
        file = open(self.file_name, 'w')
        file.write(data)
        file.close()