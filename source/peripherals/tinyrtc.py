from machine import Pin
import ujson as json

class TinyRTC ():
    def __init__ (self, sq_pin, ds, eeprom):

        self.ds = ds
        self.eeprom = eeprom
        self.sq_pin = sq_pin

        self.bytes_written = int.from_bytes(self.eeprom.read(0, 2), 'big')
        self.eeprom_objects = self.get_objects_from_eemprom()

        self.datetime = None
        self.on_second_passed = Event () 
        
        #1Hz square wave
        self.ds.square_wave(1, 0) 
        #Interrupt
        sq_pin.irq(handler=self.on_second_passed, trigger=Pin.IRQ_RISING) 

        self.ds.halt(False)

        self.on_second_passed += self.update_datetime

    def get_objects_from_eemprom (self):
        data = self.eeprom.read(2, self.bytes_written)
        objects = {}

        try:
            objects = json.loads(data)
        except:
            print('An error occour when reading data from the eeprom. Bytes written: {}'.format(self.bytes_written))

        return objects

    def reset_eemprom (self):
        self.eeprom.write(0, bytes(2))

    def write_object (self, new_obj):
        #Concat new obj
        self.eeprom_objects.update(new_obj)

        data = json.dumps(self.eeprom_objects)
        length = len(data)
        length_bytes = length.to_bytes(2, 'big')

        self.eeprom.write(0, length_bytes)
        self.eeprom.write(2, data)

    def get_object (self, key):
        obj = self.eeprom_objects[key] if key in self.eeprom_objects else None
        return obj

    def set_datetime (g):
        self.ds.datetime(g)
        self.update_datetime()

    def update_datetime ():
        self.datetime = ds.datetime()

class Event ():

    def __init__ (self):
        self.handlers = []

    def __add__ (self, a):
        self.handlers.append(a)
        return self

    def __call__ (self, *args, **kwargs):
        for handler in self.handlers:
            handler()