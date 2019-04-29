from time import sleep
import serial
from enum import Enum


class Color(Enum):
    OFF = 0
    WRONG = 1
    ACTIVE = 2
    HINT = 3
    CORRECT = 4


ser = serial.Serial('/dev/cu.usbmodem1421', 115200, timeout=0)

buttons = range(0, 16)
button_states = {key: Color.OFF for key in buttons}


def button_pressed(button_num):
    ser.write(bytes(chr(button_num + 48 + 16 * Color.ACTIVE.value), 'utf-8'))


def button_released(button_num):
    ser.write(bytes(chr(button_num + 48 + 16 * Color.OFF.value), 'utf-8'))

def toggle_button(button_num, color):
    button_states[button_num] = color
    print("lighting up:", button_num, color,
          button_num + 48 + 16 * color.value)
    ser.write(bytes(chr(button_num + 48 + 16 * color.value), 'utf-8'))


def main():
    while True:
        if ser.inWaiting() > 0:
            incoming = ser.read()
            action_num = ord(incoming) - 48
            if (action_num < 16):
                button_pressed(action_num)
            else:
                button_released(action_num - 16)


main()
