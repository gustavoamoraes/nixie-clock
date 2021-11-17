from machine import Pin
from event import Event

class RTCManager():
    def __init__ (self, sqw_pin, pcf8563):

        self.rtc = pcf8563

        #Last time got
        self.datetime = None 
        self.on_second_passed = Event () 
        
        #Set square wave to 1Hz for the each-second event.
        self.rtc.set_clk_out_frequency()
        
        #Set the event interrupt
        sqw_pin.irq(handler=lambda pin: self.on_second_passed(), trigger=Pin.IRQ_RISING) 

        #Update datetime each second
        self.on_second_passed += self.update_datetime 
    
    def set_alarm (hours=None, minutes=None):
        offset = 0

        #If time already passed offset it by a day
        if hours > self.datetime[4] and minutes > self.datetime[5]:
            offset = 24*3600

        yearday = -1
        t = utime.mktime(self.datetime + (yearday,) + offset)
        self.rtc.set_daily_alarm( hours = t[4], minutes = t[5], date=t[3])

    def set_datetime (self, g):
        self.rtc.set_datetime(g)
        self.update_datetime()

    def update_datetime (self):
        self.datetime = self.rtc.datetime()

