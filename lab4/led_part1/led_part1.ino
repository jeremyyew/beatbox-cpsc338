// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;


void setup() {
  Serial.begin (9600);
}

void loop() {
  int red_output = 100;
  int green_output = 100;
  
  analogWrite(redLED, red_output);
//  while (1) {}
//  analogWrite(redLED, 255);
//  analogWrite(greenLED, green_output);
  
  delay(500);
  // off
  analogWrite(redLED, 255);

  delay(500);
}
