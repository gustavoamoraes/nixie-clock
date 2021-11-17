from machine import I2C, Pin
import srmultiplexer
import neopixel
import _thread
import ntptime
import machine
import pcf8563
import server
import utime

from main_module import MainModule
from test_module import TestModule
# from alarm_module import Alarm
# from stopwatch_module import Stopwatch
# from clock_module import Clock
from rgbled import RGBLED
from rotary_encoder import RotaryEncoder
from button import Button
from profile import Profile
from common_funcs import digit_at
from ocilator import Ocilator
from rtc_manager import RTCManager
from json_file import JsonFile

#Led ring
ring_channel = srmultiplexer.multiplexchannel(10,3)
srmultiplexer.start([ring_channel], 255)

#Neo Pixels
# np = neopixel.NeoPixel(machine.Pin(4), 6)

#Inputs
ring_wheel = RotaryEncoder(Pin(33, Pin.IN, Pin.PULL_DOWN), Pin(32, Pin.IN, Pin.PULL_DOWN))

select_button = Button(Pin(12, Pin.IN, Pin.PULL_UP), Pin.PULL_UP, Button.BUTTON_DOWN)
select_button_value = [False]
select_button.on_button += lambda: set(select_button_value)

back_button = Button(Pin(27, Pin.IN, Pin.PULL_DOWN), Pin.PULL_DOWN, Button.BUTTON_DOWN)
back_button_value = [False]
back_button.on_button += lambda: set(back_button_value)

def set (b):
    b[0] = True

digits_register_cs_pin = machine.Pin(0)
digits_register = SPI(2)

#Ocilator
ocilator = Ocilator()

#Peripherals
digits = [
    Digit(0, Digit.constant_duty, Pin(1, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(2, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(3, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(4, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(5, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(6, Pin.OUT), ocilator),
]

config_file = JsonFile('config.json')

rgb_leds = [
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
    RGBLED(RGBLED.constant_color, ocilator),
]

#Tiny RTC
i2c = I2C(0)
rtc = pcf8563.PCF8563(i2c) 
sqw_pin = Pin(35, Pin.IN) #Square wave pin

alarm_interrupt_pin = Pin(35, Pin.IN, handler=lambda pin: trigger_alarm(), trigger=Pin.IRQ_RISING)

on_alarm_switch = lambda pin: enable_alarm_interrupt() if pin.value() else disable_alarm_interrupt()
alarm_switch_pin = Pin(35, Pin.IN, trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=on_alarm_switch)
on_alarm_switch(alarm_switch_pin)

rtc_manager = RTCManager(sqw_pin, rtc)

#Set the local time by ntptime
ntptime.settime()
t = utime.localtime(utime.mktime(utime.localtime()) - 3*3600)
g = t[0],t[1],t[2],t[6],t[3],t[4],t[5]
rtc_manager.set_datetime(g)
print('Initial datetime on DS1307 is: ', rtc_manager.datetime())

rtc_manager.on_second_passed += ocilator.reset

#Ring module
# clock = Clock(tiny_rtc); #alarm = Alarm(tiny_rtc); stopwatch = Stopwatch()
main_module = MainModule( [TestModule(), TestModule()] )

#Check for alarm each second
rtc_manager.on_second_passed += check_alarm

def trigger_alarm ():
    buildinled = Pin(2, Pin.OUT)
    buildinled.value(1)

def set_background_color (background_color):

    if len(background_color) > 3:
        return

    for value in background_color:
        if value < 0 or value > 255:
            return

    for i in range(len(digits)):
        np[i] = background_color
    
    return 1

#Profile funcs
def set_digits (n, cbs):
    for i in range(len(digits)):
        digits[i].digit = digit_at(n)
        digits[i].duty_callback = cbs[i]

def set_rgb_leds (colors, color_callbacks):
    for i in range(len(colors)):
        rgb_leds[i].base_color = colors[i]
        rgb_leds[i].color_callback = color_callbacks[i]

def set_config(json):
    
    if not 'config' in json.keys():
        print('Config object not given in json.')
        return

    old_config = config_file.objects
    new_config = json['config']

    #Set digits brightness
    if 'digits_brightness' in new_config:
        for digit in digits:
            value = new_config['digits_brightness']
            scale = max(min(value, 1.0), 0.0) #Clamp 0-1
            digit.base_duty = scale * 1024

        config_file.objects['digits_brightness'] = scale

    if 'background_color' in new_config:
        color = config['background_color']
        if set_background_color(color):
            config_file.objects['background_color'] = color

    config_file.write_changes()

def get_config():
    return config_file.objects

#Called every frame
def update_digits_forever ():
    numbers  = 0

    for i in range(len(digits)):
        #Set duty
        digits[i].pwm.duty(digit[i].duty())
        number |= digits[i].digit << (i * 4)
    
    #Write to sr
    digits_register_cs_pin.off()
    digits_register.write( bytes([numbers]) )
    digits_register_cs_pin.on()

def update_leds_forever ():
    while True:
        colors = [rgb_led.color() for rgb_led in rgb_leds]
        values = [value for color in colors for value in color]
        ring_channel.set_values(values)

def apply_profile (profile):
    if not type(profile) == Profile:
        return
        
    # set_digits (profile.display_number, profile.duty_cicles)
    set_rgb_leds(profile.ring_colors, profile.color_callbacks)

def led_ring_update_loop ():

    while True:

        profile = None

        if not (dir := ring_wheel.direction) == 0:
            profile = main_module.on_changed(dir) or profile

        if select_button_value[0]:
            profile = main_module.on_select() or profile
            select_button_value[0] = False
        
        if back_button_value[0]:
            profile = main_module.on_back() or profile
            back_button_value[0] = False

        profile = main_module.update() or profile

        apply_profile(profile)

# def start():
#     # _thread.start_new_thread (ocilator.update, ())
#     _thread.start_new_thread (first.led_ring_update_loop, ())
#     # _thread.start_new_thread (update_digits, ())
#     _thread.start_new_thread (first.update_leds, ())
#     _thread.start_new_thread (server.run_server, ())