class TinyRTC ():
    def __init__ (sq_pin, ds):
        self.datetime = None
        self.sq_pin = sq_pin
        self.ds = ds
        self.on_second_passed = Event () 
        
        #1Hz square wave 
        self.ds.square_wave(1, 0)

        #Interrupt 
        sq_pin.irq(handler=self.on_second_passed, trigger=Pin.IRQ_RISING, priority=1)

        self.tiny_rtc.on_second_passed += update_datetime

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