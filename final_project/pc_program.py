from time import sleep
import serial
from enum import Enum

class Color(Enum):
   OFF = 0
   A = 1
   B = 2
   C = 3

ser = serial.Serial('/dev/cu.usbmodem1411', 115200, timeout=0)

buttons = range(0, 16)
button_states = {key: Color.OFF for key in buttons}

def button_pressed(button_num):
   print("pressed:", button_num)
   curr_state = button_states[button_num]
   print("current state:", curr_state)
   next_state = Color((curr_state.value + 1) % len(Color))

   toggle_button(button_num, next_state)
   button_states[button_num] = next_state
   

def button_released(button_num):
   print("released:", button_num)

def toggle_button(button_num, color):
   print("lighting up:", button_num, button_num + 48 + 16 * color.value)
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

   
