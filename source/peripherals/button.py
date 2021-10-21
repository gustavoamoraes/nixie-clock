import time

class Button ():
    def __init__(self, pin, slot_count, trigger, set_interval):
        self.pin = pin
        self.pin.irq(self.set_all, trigger)
        self.slot_count = slot_count
        self.slots = [False] * slot_count
        self.slot_timers = [(0,0)] * slot_count
        self.set_interval = set_interval

    def value ():
        for i, j in enumerate(self.slots):
            if j:
                self.slots[i] = False
                return j
        return False

    def set_all ():
        for i, j in enumerate(self.slots):
            diff = self.slot_timers[i][1] - self.slot_timers[i][0]
            if diff > self.set_interval:
                self.slots[i] = True
                self.slot_timers[i][0] = time.ticks_ms()
            self.slot_timers[i][1] = time.ticks_ms()