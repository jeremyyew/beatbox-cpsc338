int trigPin = 12;
int echoPin = 13;
int analogPin = A0;
// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;
int reading;
int brightness;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  reading = analogRead(analogPin);

  // Starting range at reading = 200 
  // since we don't usually observe readings below 200
  brightness = map(reading, 200, 1023, 255, 0);
  brightness = constrain(brightness, 0, 255);
  Serial.print("reading = ");
  Serial.println(reading);
  Serial.print("brightness = ");
  Serial.println(brightness);
//  analogWrite(blueLED, brightness);
  
  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
//  Serial.print("Duration = ");
//  Serial.print(duration);
//  Serial.print(" microseconds - ");
  
  distance = (duration / 2) * 0.0344;

  distance = constrain(distance, 2, 400);
  int distance_range = 400 - 2;
  float distance_p = distance/(distance_range * 1.0);
  int inv_bright = 255 - brightness;
  int inv_red = distance_p * inv_bright;
  int inv_green = (1 - distance_p) * inv_bright;
  
//  float green_p = 1 - red_p;

  int red_output = 255 - inv_red;
  int green_output = 255 - inv_green;
  analogWrite(redLED, red_output);
  analogWrite(greenLED, green_output);
  Serial.print("red = ");
  Serial.println(red_output);
  Serial.print("green = ");
  Serial.println(green_output);
  
  if (distance >= 400 || distance <= 2){
    Serial.print("Distance = ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }
  Serial.println();
  delay(100);
}
