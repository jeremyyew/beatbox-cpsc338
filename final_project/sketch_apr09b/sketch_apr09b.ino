byte noteON = 144;//note on command
bool state;
unsigned long time;
bool notePlaying = false;
int ledON = 200;
int ledOFF = 255;
int redLED = 11;
int buttonPin = 2;

void setup()
{
  // Set up PD2 for input with passive pull-up
  pinMode(2, INPUT_PULLUP);
  
  state = false;
  time = millis();
  Serial.begin(115200);
}

void loop()
  {
    // Set pinState to the value of PD2
    bool pinState = digitalRead(buttonPin);
    if (notePlaying && millis() > time + 500) {
       MIDImessage(noteON, 41, 0);
       notePlaying = false;
       analogWrite(redLED, ledOFF);
    }
  
    // Debounce by checking if 200 ms has passed since last state change
    if (millis() > time + 50 && pinState != state) {
      if (!pinState) {
          notePlaying = true;
          MIDImessage(noteON, 41, 100);//turn note on
          analogWrite(redLED, ledON);
      }
      state = pinState;
  
      // Reset timer after state change
      time = millis();
    }
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
