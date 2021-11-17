from rgbled import RGBLED
from digit import Digit
import constants

class Profile ():
    def __init__ (self, number, duty_callbacks, ring_colors, color_callbacks):
        self.display_number = number
        self.duty_callbacks = duty_callbacks
        self.ring_colors = ring_colors
        self.color_callbacks = color_callbacks

    @staticmethod
    def default():
        return Profile(0, [Digit.constant_duty] * constants.DIGIT_COUNT, [(0,0,0)] * constants.LED_RING_COUNT, [RGBLED.constant_color] * constants.LED_RING_COUNT)