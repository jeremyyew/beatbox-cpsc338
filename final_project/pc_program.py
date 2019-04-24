from time import sleep
import serial

ser = serial.Serial('/dev/cu.usbmodem1421', 115200, timeout=0)

def button_pressed(button_num):
   print("pressed:", button_num)

def button_released(button_num):
   print("released:", button_num)

received = False
while not received:
   if ser.inWaiting() > 0: 
      incoming = ser.read()
      action_num = ord(incoming) - 48
      if (action_num < 16):
         button_pressed(action_num)
      else:
         button_released(action_num - 16)

