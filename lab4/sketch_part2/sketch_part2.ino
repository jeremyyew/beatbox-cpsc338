#include "concurrency.h"

// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;
int red_output = 100;
int green_output = 100;
  

void p1 (void)
{
  while (1) {
    analogWrite(redLED, red_output);
    delay(500);
    analogWrite(redLED, 255);
    delay(500);
  }
}

void p2 (void)
{
  while (1) {
    analogWrite(greenLED, green_output);
    delay(1000);
    analogWrite(greenLED, 255);
    delay(1000);
  }
}

void setup()
{
  Serial.begin(9600);
  if (process_create (p1, 64) < 0) {
    return;
  } 
  Serial.println("Done creating p1");
  if (process_create (p2, 64) < 0) {
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
  while (1) ;
}
