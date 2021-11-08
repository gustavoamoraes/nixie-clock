from machine import I2C, Pin
from tinyrtc import TinyRTC
import at24c32n
import ds1307
import srmultiplexer
import ntptime
import neopixel
import machine
import server

from button import Button
from main_module import MainModule
from alarm_module import Alarm
from stopwatch_module import Stopwatch
from clock_module import Clock
from rotary_encoder import RotaryEncoder
from common_funcs import digit_at

#Led ring
ring_channel = srmultiplexer.multiplexchannel(10,3)
srmultiplexer.start([ring_channel], 255)

#Neo Pixels
np = neopixel.NeoPixel(machine.Pin(4), 6)

#Inputs
ring_wheel = RotaryEncoder(Pin(33, Pin.IN, Pin.PULL_DOWN), Pin(32, Pin.IN, Pin.PULL_DOWN))

select_button = Button(Pin(12, Pin.IN, Pin.PULL_DOWN), Pin.PULL_DOWN, Button.BUTTON_DOWN)
select_button_value = [False]
select_button.on_button += lambda: set(select_button_value)

back_button = Button(Pin(33, Pin.IN, Pin.PULL_DOWN), Pin.PULL_DOWN, Button.BUTTON_DOWN)
back_button_value = [False]
back_button.on_button += lambda: set(back_button_value)

def set (b):
    b[0] = True

#Outputs
digits_register_cs_pin = machine.Pin(0)

#Peripherals
digits = [
    Digit(0, Digit.constant_duty, Pin(1, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(2, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(3, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(4, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(5, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(6, Pin.OUT), ocilator),
]

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

digits_register = SPI(2)

#Tiny RTC
i2c = I2C(1)
ds = ds1307.DS1307(i2c)
eeprom = at24c32n.AT24C32N(i2c)
sq_pin = Pin(33, Pin.IN)
tiny_rtc = TinyRTC(sq_pin, ds, eeprom)

#Set the local time by ntptime
ntptime.settime()
t = utime.localtime(utime.mktime(utime.localtime()) - 3*3600)
g = t[0],t[1],t[2],t[6],t[3],t[4],t[5]
tiny_rtc.set_datetime(g)
print('Initial datetime on DS1307 is: ', tiny_rtc.datetime)

ocilator = Ocilator()
tiny_rtc.on_second_passed += ocilator.reset

#Ring module
clock = Clock(tiny_rtc); #alarm = Alarm(tiny_rtc); stopwatch = Stopwatch()
main_module = MainModule( [clock] )

#Check for alarm each second
tiny_rtc.on_second_passed += check_alarm

#Alarm
def check_alarm ():
    time = alarm.time
    minute = time[0]
    seconds = time[1]

    if minute == tiny_rtc.datetime[1] and seconds == tiny_rtc.datetime[0] :
        trigger_alarm()

def trigger_alarm ():
    buildinled = Pin(2, Pin.OUT)
    buildinled.value(1)

def set_background_color (background_color):
    for i in range(len(digits)):
        np[i] = background_color

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

    old_config = get_config()
    new_config = json['config']

    #Set digits brightness
    if 'digits_brightness' in config:
        for digit in digits:
            val = new_config['digits_brightness']
            scale = max(min(val, 1.0), 0.0)
            digit.base_duty = scale * 1024

        old_config['digits_brightness'] = val

    if 'background_color' in config:
        val = config['background_color']
        set_background_color(val)
        old_config['background_color'] = val

    tiny_rtc.write_obj(config)

def get_config():
    text = tiny_rtc.get_object('config')
    obj = json.loads(text)
    return obj

#Called every frame
def update_digits ():
    numbers  = 0

    for i in range(len(digits)):
        #Set duty
        digits[i].pwm.duty(digit[i].duty)
        number |= digits[i].digit << i
    
    #Write to sr
    digits_register_cs_pin.off()
    digits_register.write( bytes([numbers]) )
    digits_register_cs_pin.on()

def update_leds ():
    values = [color for color in rgb_led.color for rgb_led in rgb_leds]
    ring_channel.set_values(values)

def apply_profile (profile):
    if not profile == type(Profile):
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

#Create thread for
    #ocilator.update()
    #led_ring_update_loop()
    #update_digits ()
    #update_leds()
    #server.run_server()

