#include "testasm.h"

void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);

  // Test no carry
  Serial.print("The sum of 3 and 4 is: ");
  Serial.print(testasm(3, 4));
  Serial.println(" (Expected 7)");
  Serial.print("The sum of 4 and 3 is: ");
  Serial.print(testasm(4, 3));
  Serial.println(" (Expected 7)");

  // Test simple carry
  Serial.print("The sum of 255 and 1 is: ");
  Serial.print(testasm(255, 1));
  Serial.println(" (Expected 256)");
  Serial.print("The sum of 1 and 255 is: ");
  Serial.print(testasm(1, 255));
  Serial.println(" (Expected 256)");

  // Test carry
  Serial.print("The sum of 255 and 255 is: ");
  Serial.print(testasm(255, 255));
  Serial.println(" (Expected 510)");

  pinMode(13, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(400);
  digitalWrite(13, LOW);
  delay(400);
}
