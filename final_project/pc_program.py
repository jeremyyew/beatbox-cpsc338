from time import sleep
import serial

ser = serial.Serial('/dev/cu.usbmodem141201', 9600, timeout=0)

received = False
while not received:
   print("p1 sending")
   ser.write("hello?\r".encode()) # Convert the decimal number to ASCII then send it to the Arduino
   if ser.inWaiting() > 0: 
      incoming = ser.readline().decode()
      print("incoming:"+incoming)
      print(ascii(incoming))
      if incoming == "hey!\r\n":
         received = True
   sleep(1) # Delay for one tenth of a second
print("received") # Read the newest output from the Arduino
