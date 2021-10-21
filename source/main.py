from machine import I2C, Pin
from tinyrtc import TinyRTC
import at24c32n
import ds1307
import srmultiplexer

#import uasyncio
#import neopixel
#import socket
import machine
import os

from button import Button
from main_module import MainModule
from alarm_module import Alarm
from stopwatch_module import Stopwatch
from clock_module import Clock
from rotary_encoder import RotaryEncoder
from common_funcs import digit_at


# HTML Server
# page_text = open('page.html', 'r').read()
# addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

# s = socket.socket()
# s.bind(addr)
# s.listen(1)

# print('listening on', addr)

#Led ring
ring_channel = srmultiplexer.multiplexchannel(10,3)
srmultiplexer.start([ring_channel], 255)

#Neo Pixels
#np = neopixel.NeoPixel(machine.Pin(4), 6)

#Inputs
ring_wheel = RotaryEncoder(33, 32)
select_button = Button(Pin(12, Pin.IN, Pin.PULL_UP), 1, Pin.FALLING, 50)
back_button = Button(Pin(32, Pin.IN, Pin.PULL_UP), 1, Pin.FALLING, 50)

#Outputs
# digits_register_cs_pin = machine.Pin(0)

# #Settings#
# current_duty = 1024
# background_color = (50,0,50)

ocilator = Ocilator()
'''
digits = [
    Digit(0, Digit.constant_duty, Pin(1, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(2, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(3, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(4, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(5, Pin.OUT), ocilator),
    Digit(0, Digit.constant_duty, Pin(6, Pin.OUT), ocilator),
]
'''
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

#digits_register = SPI(2)

#Tiny RTC
i2c = I2C(1)
ds = ds1307.DS1307(i2c)
sq_pin = Pin(33, Pin.IN)
eeprom = at24c32n.AT24C32N(i2c)
tiny_rtc = TinyRTC(sq_pin, ds, eeprom)

#Set the time by ntptime
# ntptime.settime(timezone=-3)
# f = utime.localtime()
# g = f[0],f[1],f[2],f[6],(f[3]+8),f[4],f[5]
# tiny_rtc.set_datetime(g)

#print('initial datetime on ds1307 is...', tiny_rtc.datetime)

#tiny_rtc.on_second_passed += ocilator.reset

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

    if minute == tiny_rtc.datetime[1] and tiny_rtc.datetime[2] == seconds :
        trigger_alarm()

def trigger_alarm ():
    buildinled = Pin(2, Pin.OUT)
    buildinled.value(1)

def set_background_color ():
    for i in range(len(digits)):
        np[i] = background_color

#Profile funcs
def set_digits (n, cbs):
    for i in range(len(digits)):
        digits[i].digit = digit_at(n)
        digits[i].duty_callback = cbs[i]

def set_rgb_leds (colors, color_callbacks):
    colors_length = len(colors)
    for i in range(colors_length):
        rgb_leds[i].base_color = colors[i]
        rgb_leds[i].color_callback = color_callbacks[i]

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
        
    #set_digits (profile.display_number, profile.duty_cicles)
    set_rgb_leds(profile.ring_colors, profile.color_callbacks)

def server():

    while True:

        cl, addr = s.accept()
        print('client connected from', addr)
        cl_file = cl.makefile('rwb', 0)

        length = 0

        while True:
            line = cl.readline()
            if not line or line == b'\r\n':
                break
            if 'Content-Length: ' in line:
                length = int(line[16:-2])

        cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')

        if length:
            postquery = cl.read(length)
            request = ujson.loads(postquery)
                
        cl.sendall(page_text)
        cl.close()

def button_timer (timers, interval, boolean):


def main ():

    while True:
        
        ocilator.update()

        profile = None

        if not (dir := ring_wheel.direction) == 0:
            profile = main_module.on_changed(dir) or profile

        if select_button.value():
            profile = main_module.on_select() or profile
        
        if back_button.value():
            profile = main_module.on_back() or profile

        profile = main_module.update() or profile

        #apply_profile(profile)