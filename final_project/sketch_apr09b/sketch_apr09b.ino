byte noteON = 144;//note on command
bool state;
unsigned long time;
bool notePlaying = false;

void setup()
{
  // Set up PD2 for input with passive pull-up
  PORTD |= (1 << PD2);
  DDRD &= ~(1 << PD2);
  
  state = false;
  time = millis();
  Serial.begin(115200);
}

void loop()
  {
    // Set pinState to the value of PD2
    bool pinState = (PIND & (1 << PD2));
    if (notePlaying && millis() > time + 500) {
//       MIDImessage(noteON, 60, 0);
       notePlaying = false;
    }
  
    // Debounce by checking if 200 ms has passed since last state change
    if (millis() > time + 50 && pinState != state) {
      if (!pinState) {
          notePlaying = true;
          MIDImessage(noteON, 60, 100);//turn note on
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
