from ring_module import RingModule

class Clock (RingModule):

    def __init__(self, rtc_manager):
        super().__init__("Stopwatch")

        self.rtc_manager = rtc_manager
        self.rtc_manager.on_second_passed += lambda: setattr(self, 'update', True)

        self.update = False

    def on_back(self):
        super().on_back()
           
    def update (self):
        if self.update:
            datetime = self.rtc_manager.datetime
            self.profile.display_number = (datetime[2] * 10000)  + (datetime[1] * 100) + datetime[0]
            self.update = False
            return self.profile