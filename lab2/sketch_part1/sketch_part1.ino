bool state;
unsigned long time;

void setup()
{
  // Set up PD2 for input with passive pull-up
  PORTD |= (1 << PD2);
  DDRD &= ~(1 << PD2);
  
  state = false;
  time = millis();
  Serial.begin(9600);
}

void loop()
{
  // Set pinState to the value of PD2
  bool pinState = (PIND & (1 << PD2));

  // Debounce by checking if 200 ms has passed since last state change
  if (millis() > time + 200 && pinState != state) {
    if (pinState) {
        Serial.println("Release (OFF)");
    } else {
        Serial.println("Press (ON)");
    }
    state = pinState;

    // Reset timer after state change
    time = millis();
  }
}
