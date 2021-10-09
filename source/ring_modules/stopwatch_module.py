from ring_module import RingModule
import time

class Stopwatch(RingModule):

    def __init__(self):
        super().__init__("Stopwatch")

        self.running = False
        self.ticks_on_start = 0
        self.ticks_on_stop = 0

    def on_select ():
        if !self.running:
            if self.profile.display_number == 0:
                self.ticks_on_start = time.ticks_ms()
                self.runnuing = True
            else:
                self.profile.display_number = 0
        else:
            self.running = False
        
        return self.profile

    @property
    def time_number ():
        micro_s_diff = time.ticks_diff(ticks_on_stop, ticks_on_start)

        ms = micro_s_diff/10
        s = micro_s_diff/1000
        m = micro_s_diff/1000*60

        return ( (m%60) * 1000 ) + ( (s%60) * 100 ) + (ms%100)

    def update():
        if self.running:
            self.ticks_on_stop = time.ticks_ms()
            self.profile.display_number = self.time_number
            
            return self.profile
