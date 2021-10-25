from event import Event
from machine import Pin

class RotaryEncoder ():
    def __init__(self, clk_pin, dt_pin):

        self.dt_pin = dt_pin
        self.clk_pin = clk_pin
        self.last_clk_value = clk_pin.value()

    @property
    def direction(self):
        clk_value = self.clk_pin.value()
        dir =  (1 if clk_value == self.dt_pin.value() else -1) if not clk_value == self.last_clk_value else 0 
        self.last_clk_value = clk_value
        return dir
