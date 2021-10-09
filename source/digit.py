from machine import PWM

class Digit:

    def __init__(self, digit, duty_callback, pwm_pin):

        self.digit = digit
        self.pwm = PWM( Pin(pwm_pin, Pin.OUT) )
        self.duty_callback = duty_callback
        self.current_duty = 1024;

    @property
    def duty():
        return duty_callback()

    def oscillating_duty ():
        return (sin(seconds_timer * ocillation_freq) + 1) / 2 * current_duty

    def constant_duty ():
        return current_duty