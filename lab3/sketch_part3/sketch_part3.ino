//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

// Arduino digital pin numbers
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 128; // Starting value - corresponding to velocity = 0

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Serial.begin (9600);
  
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

}

void loop(){
  Serial.print("reading = ");
  Serial.println(encoderValue);
  delay(1000); //just here to slow down the output, and show it will work even during a delay
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  
  int encoded = (MSB << 1) | LSB; //converting the 2 pin values to 2 bit number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value 

  // Checking valid transition states
  // TODO: is this clockwise or counter-clockwise?
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
    encoderValue = constrain(encoderValue, 0, 255); // Saturating end points
  }

  // Checking valid transition states
  // TODO: is this clockwise or counter-clockwise?
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--; 
    encoderValue = constrain(encoderValue, 0, 255); // Saturating end points
  }
  lastEncoded = encoded; //store this value for next time 
}
