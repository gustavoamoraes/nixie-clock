from math import sin
import time

#Ocilates between 0 and 1. Use for the pwm animations
class Ocilator ():

    def __init__(self):
        self.start_time = 0
        self.end_time = 0

    def reset (self):
        self.start_time = time.ticks_ms()

    def update_forever (self):
        while True:
            self.end_time = time.ticks_ms()

    def seconds (self):
        return time.ticks_diff(self.end_time, self.start_time)/1000

    def positive_ocilation (self, f, a, o):
        return (sin(o + (self.seconds() * f * math.pi * 2)) + 1) / 2 * a