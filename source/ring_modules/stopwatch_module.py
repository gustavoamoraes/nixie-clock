from ring_module import RingModule
import time

class Stopwatch(RingModule):

    def __init__(self):
        super().__init__("Stopwatch")

        self.running = False
        self.ticks_on_start = 0
        self.ticks_on_stop = 0

    def on_select (self):
        if not self.running:
            if self.profile.display_number == 0:
                self.ticks_on_start = time.ticks_ms()
                self.runnuing = True
            else:
                self.profile.display_number = 0
        else:
            self.running = False
        
        return self.profile

    def time (self):
        ms_diff = time.ticks_diff(ticks_on_stop, ticks_on_start)
        milliseconds = ms_diff/10; seconds = (ms_diff/1000)%60; minutes = (seconds*60)%60
        return ( minutes * 1000 ) + ( seconds * 100 ) + (milliseconds%100)

    def update(self):
        if self.running:
            self.ticks_on_stop = time.ticks_ms()
            self.profile.display_number = self.time()
            return self.profile
