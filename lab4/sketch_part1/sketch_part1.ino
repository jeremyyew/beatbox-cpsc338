#include "concurrency.h"

/*
In this example, we have two processes concurrently write to two different pins in an RGB LED. Since the pins are not a shared resource, the results of writing to them concurrently is predictable. Due to the overlapping intervals of blinking, the expected result is for the light to blink in the following pattern, in a cycle: 
  RG -> XG -> RX -> XX
where X means no light for that pin.
*/
// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;

void p1(void)
{
  while (1)
  {
    analogWrite(redLED, 100);
    delay(500);
    analogWrite(redLED, 255);
    delay(500);
  }
}

void p2(void)
{
  while (1)
  {
    analogWrite(greenLED, 100);
    delay(1000);
    analogWrite(greenLED, 255);
    delay(1000);
  }
}

void setup()
{
  Serial.begin(9600);
  if (process_create(p1, 64) < 0)
  {
    return;
  }
  Serial.println("Done creating p1");
  if (process_create(p2, 64) < 0)
  {
    return;
  }
  Serial.println("Done creating p2");
}

void loop()
{
  process_start();
  Serial.println("Processes done");
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1)
    ;
}
