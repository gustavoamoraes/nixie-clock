class Clock (RingModule):

    def __init__(self, tiny_rtc):
        super().__init__("Stopwatch")

        self.tiny_rtc = tiny_rtc
        self.tiny_rtc.on_second_passed += lambda: setattr(self, 'update', True)

        self.update = False
            
    def update (self):
        if self.update:
            datetime = tiny_rtc.datetime
            self.profile.display_number = (datetime[1] * 10000)  + (datetime[1] * 100) + datetime[0]
            self.update = False

            return self.profile