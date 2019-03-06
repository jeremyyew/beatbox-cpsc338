// TODO:
// 2. Jeremy: circuit part 2
// 3. part 2 video 

// Rangefinder code referenced from http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/

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

  brightness = map(reading, 0, 1023, 255, 0);
  // brightness = constrain(brightness, 0, 255); 
  Serial.print("reading = ");
  Serial.println(reading);
  Serial.print("brightness = ");
  Serial.println(brightness);
  
  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Distance in cm is one-way duration (in microsecond) times speed of sound (in cm per microsecond),
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;

  // Convert distance to percentage of max distance. 
  int distance_range = 400 - 2;
  float distance_p = distance/(distance_range * 1.0);

  // Calculate proportion of red and green values, which should sum to inv_bright. Red is far, green is close. 
  int inv_bright = 255 - brightness;
  int inv_red = distance_p * inv_bright;
  int inv_green = (1 - distance_p) * inv_bright;
  
  int red_output = 255 - inv_red;
  int green_output = 255 - inv_green;
  analogWrite(redLED, red_output);
  analogWrite(greenLED, green_output);

  Serial.print("red = ");
  Serial.println(red_output);
  Serial.print("green = ");
  Serial.println(green_output);
  
  // This is the effective range of the rangefinder
  if (distance >= 400 || distance <= 2){
    Serial.print("Distance = ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  Serial.println();
  delay(500);
}
