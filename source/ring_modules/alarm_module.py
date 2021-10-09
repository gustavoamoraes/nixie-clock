from ring_module import RingModule
import constants

class Alarm(RingModule):

    def __init__(self, ):
        super().__init__("Alarm")

        self.ring_index = 0
        self.digit_index = 0

        self.set_number = 0
        self.current_number = 0

    #Return a tupple with (HH, MM, SS) of the alarm time set
    @property
    def time():
        return 
        (
            digit_at(self.set_number, 5) * 10
            + digit_at(self.set_number, 4),

            digit_at(self.set_number, 3) * 10
            + digit_at(self.set_number, 2),

            digit_at(self.set_number, 1) * 10
            + digit_at(self.set_number, 0),
        )

    def on_changed (q):
        self.ring_index = (self.ring_index + q) % 10
        place = pow(10, ring_index)
        self.current_number = self.set_number - (place * digit_at(self.set_number, self.ring_index)) 
        + (place * self.ring_index)

        #Show alarm time being choosen
        self.profile.display_number = self.current_number

        return self.profile

    def on_select ():

        self.digit_index += 1
        self.set_number = self.current_number
        self.ring_index = digit_at(self.current_number, self.digit_index)

        #Highlighting digit 
        self.profile.duty_cicles = [ Digit.constant_duty for i in range(constants.DIGIT_COUNT) 
        if i != self.digit_index else Digit.oscillating_duty]

        #Show alarm time
        self.profile.display_number = set_number
        
        return self.profile