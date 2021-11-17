from machine import PWM

class Digit:

    def __init__(self, digit, duty_callback, pwm_pin, ocilator):

        self.digit = digit
        self.pwm = PWM(pwm_pin)
        self.duty_callback = duty_callback
        self.base_duty = 1024;
        self.ocilator = ocilator

    def duty():
        return duty_callback()

    def oscillating_duty ():
        return self.ocilator.positive_ocilation(1, self.current_duty)

    def constant_duty ():
        return current_duty