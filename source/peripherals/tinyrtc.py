from machine import Pin
import ujson as json
from event import Event

class TinyRTC ():
    def __init__ (self, sq_pin, ds, eeprom):

        self.ds = ds
        self.eeprom = eeprom
        self.sq_pin = sq_pin

        self.bytes_written = int.from_bytes(self.eeprom.read(0, 2), 'big') #Bytes written to the eeprom excluding the first 2
        self.eeprom_objects = self.get_objects_from_eemprom() #All objects written to the eemprom

        self.datetime = None #Last time got
        self.on_second_passed = Event () 
        
        #Set square wave to 1Hz for the each-second event.
        self.ds.square_wave(1, 0) 
        
        #Set the event interrupt
        sq_pin.irq(handler=self.on_second_passed, trigger=Pin.IRQ_RISING) 

        self.ds.halt(False)

        #Update datetime each second
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
        return self.eeprom_objects[key] if key in self.eeprom_objects else None

    def set_datetime (g):
        self.ds.datetime(g)
        self.update_datetime()

    def update_datetime ():
        self.datetime = ds.datetime()

