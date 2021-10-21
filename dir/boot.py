# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)

def do_connect():
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('Connecting to network...')
        sta_if.active(True)
        sta_if.connect('AUREO', 'BananaCanela')
        while not sta_if.isconnected():
            pass
    print('network config:', sta_if.ifconfig())
    
import webrepl
do_connect()
webrepl.start()
