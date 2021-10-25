import time
from machine import Pin
from event import Event

class Button ():

    BUTTON_DOWN = 1
    BUTTON_UP = 2
    DELAY = 50

    def __init__(self, pin, pin_pull, button_mode):

        pin.irq(self.input_handler, Pin.IRQ_FALLING | Pin.IRQ_RISING)

        self.on_button = Event()
        self.button_mode = button_mode
        self.on_state = not (pin_pull - 1)
        self.off_timer = 0
        self.on_timer = 0
        self.last_state = 1
    
    def input_handler (self, pin):
        if pin.value() == self.on_state:
            off_diff = time.ticks_ms() - self.off_timer

            if off_diff > Button.DELAY and self.last_state == (not self.on_state):
                
                if self.button_mode == 1:
                    self.on_button()

                self.on_timer = time.ticks_ms()
                self.last_state = self.on_state
        else:
            on_diff = time.ticks_ms() - self.on_timer

            if on_diff > Button.DELAY and self.last_state == self.on_state:

                if self.button_mode == 2:
                    self.on_button()

                self.off_timer = time.ticks_ms()
                self.last_state = not self.on_state