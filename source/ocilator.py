import math

class Ocilator ():

    def __init__(self):
        self.start_time = 0
        self.end_time = 0

    def reset (self):
        start_time = time.ticks_ms()

    def update (self):
        end_time = time.ticks_ms()

    @property
    def time_s ():
        return time.ticks_diff(end_time, start_time)/1000

    def positive_ocilation (f, a, o):
        return (sin(o + (self.time_s * f * math.pi * 2)) + 1) / 2 * a