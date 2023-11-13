from time import sleep

from machine import Pin

from machine import PWM
from machine import Timer

MAX_DUTY_CYCLE = 2300
MIN_DUTY_CYCLE = 700


pwm0 = PWM(Pin(0))
pwm1 = PWM(Pin(1))
pwm2 = PWM(Pin(2))

pwm0.freq(50)
pwm1.freq(50)
pwm2.freq(50)

led = Pin(25, Pin.OUT)

tim = Timer(period=50, mode=Timer.PERIODIC, callback=lambda t:led.toggle())

#The position is expected as a parameter

def setServoCycle (angle):
    position = int((angle/180.0) * (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) + MIN_DUTY_CYCLE)
    pwm0.duty_u16(position)
    pwm1.duty_u16(position)
    pwm2.duty_u16(position)
    sleep(0.03)

while True:
    for pos in range(0,180,1):
        setServoCycle(pos)

    for pos in range(180,0,-1):
        setServoCycle(pos)

