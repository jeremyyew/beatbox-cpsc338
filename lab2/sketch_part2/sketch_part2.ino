int latchPin = 12;
int clockPin = 11;
int dataPin = 13;

void setup() {
  // put your setup code here, to run once:
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, B00001010); 
   digitalWrite(latchPin, HIGH);
   Serial.print("hi");
   delay(250);
}
