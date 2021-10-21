class Profile ():
    def __init__ (number, duty_callback, ring_colors):
        self._display_number = number
        self._duty_callbacks = duty_callbacks
        self._ring_colors = ring_colors

    @staticmethod
    def default():
        return Profile(0, [Digit.constant_duty for i in range( constants.DIGIT_COUNT )])

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