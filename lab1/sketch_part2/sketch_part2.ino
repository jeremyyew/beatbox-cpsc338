#include "testasm.h"

void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);

  // Test simple case
  Serial.print("sumval(0,0)+diffval(0,0): ");
  Serial.print(testasm(0, 0));
  Serial.println(" (Expected: 0)");

  Serial.print("sumval(3,4)+diffval(3,4): ");
  Serial.print(testasm(3, 4));
  Serial.println(" (Expected: 8)");

  // Test carry addition
  Serial.print("sumval(255,2)+diffval(255,2): ");
  Serial.print(testasm(255, 2));
  Serial.println(" (Expected: 510)");

  // Test reverse order carry addition
  Serial.print("sumval(2,255)+diffval(2,255): ");
  Serial.print(testasm(2, 255));
  Serial.println(" (Expected: 510)");

  // Test with neg
  Serial.print("sumval(5, -1) + diffval(5, -1):");
  Serial.print(testasm(5, -1));
  Serial.println(" (Expected: 510) due to two's complement");

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
