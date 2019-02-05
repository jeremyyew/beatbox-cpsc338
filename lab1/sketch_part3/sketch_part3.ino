#include "testasm.h"
#include "stdio.h"
void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);
  for (int i = 0; i < 25; i++)
  {
    Serial.print("\nfib(");
    Serial.print(i);
    Serial.print(")=\n");
    Serial.print(testasm(i, 0));
  }
  // Serial.print("\nfib(0)=\n");
  // Serial.print(testasm(0, 0));
  // Serial.print("\nfib(1)=\n");
  // Serial.print(testasm(1, 0));
  // Serial.print("\nfib(2)=\n");
  // Serial.print(testasm(2, 0));
  // Serial.print("\nfib(3)=\n");
  // Serial.print(testasm(3, 0));
  // Serial.print("\nfib(4)=\n");
  // Serial.print(testasm(4, 0));
  // Serial.print("\nfib(5)=\n");
  // Serial.print(testasm(5, 0));
  // Serial.print("\nfib(6)=\n");
  // Serial.print(testasm(6, 0));
  // Serial.print("\nfib(7)=\n");
  // Serial.print(testasm(7, 0));
  // Serial.print("\nfib(8)=\n");
  // Serial.print(testasm(8, 0));
  // Serial.print("\nfib(9)=\n");
  // Serial.print(testasm(9, 0));
  // Serial.print("\nfib(10)=\n");
  // Serial.print(testasm(10, 0));
  // Serial.print("\nfib(10)=\n");
  // Serial.print(testasm(10, 0));
  // Serial.print("\nfib(13)=\n");
  // Serial.print(testasm(13, 0));
  // Serial.print("\nfib(24)=\n");
  // Serial.print(testasm(24, 0));
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
