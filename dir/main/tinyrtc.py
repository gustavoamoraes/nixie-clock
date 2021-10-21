import at24c32n #https://github.com/mcauser/micropython-tinyrtc-i2c
import ds1307 #https://github.com/mcauser/micropython-tinyrtc-i2c
import json

class TinyRTC ():
    def __init__ (sq_pin, ds, eeprom):

        self.ds = ds
        self.eeprom = eeprom
        self.sq_pin = sq_pin
        self.bytes_written = int.from_bytes( eeprom.read(0, 4), 'big')

        self.datetime = None
        self.on_second_passed = Event () 
        
        #1Hz square wave 
        self.ds.square_wave(1, 0)

        #Interrupt 
        sq_pin.irq(handler=self.on_second_passed, trigger=Pin.IRQ_RISING, priority=1)

        self.tiny_rtc.on_second_passed += update_datetime

    def write_object (new_obj):

        bytes_count = 0

        for key in new_obj.keys():

            json = json.dump(new_obj)
            new_obj_size = len(json)

            if not (old_obj := get_object(key)):
                self.eeprom.write(self.bytes_written, json)
                bytes_count += new_obj_size
            else:
                if old_obj.size == new_obj_size:
                    self.eeprom.write(old_obj.addr, json)
                else:
                    self.eeprom.move (old_obj.addr + old_obj.size, self.bytes_written, old_obj.addr)
                    self.eeprom.write(old_obj.addr, json)

                    bytes_count += new_obj_size - old_obj.size

        self.bytes_written += bytes_count
        self.eeprom.write(0, self.bytes_written.to_bytes(4, 'big'))

    def get_object (key):
        json = self.eeprom.read(4, self.bytes_written)
        dict = json.loads(json)

        try:
            obj = dict[key]
        except:
            obj = None
        
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