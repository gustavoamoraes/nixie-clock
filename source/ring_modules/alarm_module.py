from ring_module import RingModule
from json_file import JsonFile
import constants

class Alarm(RingModule):

    def __init__(self, rtc_manager):
        super().__init__("Alarm")

        self.rtc_manager = rtc_manager

        self.ring_index = 0
        self.digit_index = 0

        self.config_file = JsonFile('config.json')
        self.set_number = config_file.objects['alarm_time']
        self.current_number = 0

    #Return a tupple with (HH, MM) of the alarm time set
    def set_time():
        return (digit_at(self.set_number, 3) * 10 + digit_at(self.set_number, 2), 
        digit_at(self.set_number, 5) * 10 + digit_at(self.set_number, 4),)

    def on_changed (self, q):
        self.ring_index = (self.ring_index + q) % constants.LED_RING_COUNT
        self.current_number = self.set_number - ((place := pow(10, ring_index))  * digit_at(self.set_number, self.ring_index)) + (place * self.ring_index)

        #Show alarm time being choosen
        self.profile.display_number = self.current_number

        return self.profile
    
    def on_back(self):
        super().on_back()
        
    def on_select (self):

        self.digit_index += (2 + ((self.digit_index + 1) % 4)) 
        self.ring_index = digit_at(self.current_number, self.digit_index) 
        self.set_number = self.current_number

        alarm_time = self.set_time()
        self.config_file.objects['alarm_time'] = alarm_time
        self.rtc_manager.set_alarm(hours=alarm_time[0], minutes=alarm_time[1])
        self.config_file.write_changes()

        #Highlighting digit 
        self.profile.duty_cicles = [ (Digit.constant_duty for i in range(constants.DIGIT_COUNT)) 
        if not i == self.digit_index else Digit.oscillating_duty]

        #Show alarm time
        self.profile.display_number = set_number
        
        return self.profile