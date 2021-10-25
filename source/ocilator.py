from math import sin
import time

class Ocilator ():

    def __init__(self):
        self.start_time = 0
        self.end_time = 0

    def reset (self):
        self.start_time = time.ticks_ms()

    def update (self):
        self.end_time = time.ticks_ms()

    @property
    def seconds (self):
        return time.ticks_diff(self.end_time, self.start_time)/1000

    def positive_ocilation (self, f, a, o):
        return (sin(o + (self.seconds * f * math.pi * 2)) + 1) / 2 * a