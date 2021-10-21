class Profile ():
    def __init__ (number, duty_callback, ring_colors, color_callbacks):
        self.display_number = number
        self.duty_callbacks = duty_callbacks
        self.ring_colors = ring_colors
        self.color_callbacks = color_callbacks

    @staticmethod
    def default():
        return Profile(0, [Digit.constant_duty] * constants.DIGIT_COUNT, [(0,0,0)] * constants.LED_RING_COUNT, [RGBLED.constant_color] * constants.LED_RING_COUNT)

    '''
    #Custom set function that calls the callback when any member is changed
    def set_member(self, member_name, new_value):
        setattr(self, member_name, new_value)
        self.apply_profile()
        return new_value

    display_number = property(lambda self: self._display_number, (lambda self, value: set_member(self, '_display_number', value)))
    duty_callbacks = property(lambda self: self._duty_callbacks, (lambda self, value: set_member(self, '_duty_callbacks', value)))
    ring_colors = property(lambda self: self._ring_colors, (lambda self, value: set_member(self, '_ring_colors', value)))
    '''