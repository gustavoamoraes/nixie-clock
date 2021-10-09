from main_module import MainModule
from machine import I2C, Pin
import srmultiplexer
import uasyncio
import neopixel
import at24c32n #https://github.com/mcauser/micropython-tinyrtc-i2c
import ds1307 #https://github.com/mcauser/micropython-tinyrtc-i2c
import socket
import machine
import ujson
import os

from alarm_module import Alarm
from stopwatch_module import Stopwatch
from clock_module import Clock

# HTML Server
page_text = open('page.html', 'r').read()
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

s = socket.socket()
s.bind(addr)
s.listen(1)

print('listening on', addr)

#Led ring
ring_channel = srmultiplexer.multiplexchannel(10,3)
srmultiplexer.start([ring_channel])

#Neo Pixels
np = neopixel.NeoPixel(machine.Pin(4), 6)

#Inputs
ring_clockwise = Pin(0, Pin.IN)
ring_counterclockwise = Pin(1, Pin.IN)
select_button = Pin(2, Pin.IN)
back_button = Pin(3, Pin.IN)

#Outputs
digits_register_cs_pin = machine.Pin(0)

#Settings#
current_duty = 1024
background_color = (50,0,50)

digits = 
[
    Digit(0, Digit.constant_duty, 1),
    Digit(0, Digit.constant_duty, 2),
    Digit(0, Digit.constant_duty, 3),
    Digit(0, Digit.constant_duty, 4),
    Digit(0, Digit.constant_duty, 5),
    Digit(0, Digit.constant_duty, 6),
]

digits_register = SPI(2)

i2c = I2C(1)
sq_pin = Pin(15, Pin.IN)
ds = ds1307.DS1307(i2c)

print('initial datetime on ds1307 is...', ds.datetime())

tiny_rtc = TinyRTC(sq_pin, ds)

print('ntp time is set at')
#Set the time by ntptime
ntptime.settime(timezone=-3)
f = utime.localtime()
g = f[0],f[1],f[2],f[6],(f[3]+8),f[4],f[5]

tiny_rtc.set_datetime(g)

clock = Clock(tiny_rtc); alarm = Alarm(); stopwatch = Stopwatch()
main_module = MainModule( [clock, alarm, stopwatch], apply_profile) )


def check_alarm ():


def digit_at (n, i):
    return int ( (n % pow(10, i+1)) - (n % pow(10, i) ) / pow(10, i) )

#Profile funcs
def set_digits (n, duty_cicles):
    for i in range(len(digits)):
        digits[i].digit = digit_at(n)
        digits[i].duty_callback = duty_cicles[i]

def set_background_color ():
    for i in range(len(digits)):
        np[i] = background_color

def apply_profile (profile):
    if not profile == type(Profile):
        return

    set_digits (profile.display_number, profile.duty_cicles)
    ring_channel.set_values(self.ring_colors

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

def main ():

    uasyncio.create_task(server()) 

    while True:
        
        last_profile = None

        if ring_clockwise.value() or ring_counterclockwise.value():
            change = ring_clockwise.value() - ring_counterclockwise.value()
            last_profile = profile if (profile := main_module.on_changed(change)) else last_profile

        if select_button.value():
            last_profile = profile if (profile := main_module.on_select()) else last_profile
        
        if back_button.value():
            last_profile = profile if (profile := main_module.on_back()) else last_profile

        last_profile = profile if (profile := main_module.update()) else last_profile

        apply_profile(last_profile)
        

