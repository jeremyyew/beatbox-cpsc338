int analogPin = A0;
// RGB pins mapped to digital pins 11, 10, 9 respectively
int LED = 9;
int reading;
int brightness;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    reading = analogRead(analogPin);
    Serial.print("reading = ");
    Serial.println(reading);
    brightness = map(reading, 0, 1023, 255, 0);
    analogWrite(LED, brightness);
    delay(500);
}
