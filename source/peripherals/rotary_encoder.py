from event import Event
from machine import Pin

class RotaryEncoder ():
    def __init__(self, clk_pin, dt_pin):

        self.on_step = Event()
        self.dt_pin = dt_pin
        self.clk_pin = clk_pin

        self.clk_pin.irq(handler=lambda pin: self.on_step (self.direction), trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING)

    @property
    def direction(self):
        return (1 if self.clk_pin.value() == self.dt_pin.value() else -1)
