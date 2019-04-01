#include "concurrency.h"

// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;
int red_output = 100;
int green_output = 100;
  

void p1 (void)
{
  /* process 1 here */
  while (1) {
    analogWrite(redLED, red_output);
//    delay(1000);
    for (int i = 0; i < 20000;) {
      i += 1;
    }
    analogWrite(redLED, 255);
        for (int i = 0; i < 20000;) {
      i += 1;
    }
//    delay(1000);
//    Serial.print("1");
  }

//  int iteration = 0;
//  while (1) {
//    for (int i = 0; i < 5000;) {
//      i += 1;
//    }
//    if (iteration % 2 == 0) {
//      analogWrite(redLED, 255);
//    } else {
//      analogWrite(redLED, red_output);
//    }
//    iteration += 1;
//  }
  
  return;
}

void p2 (void)
{
/* process 2 here */
  while (1) {
    analogWrite(greenLED, green_output);
    for (int i = 0; i < 20000;) {
      i += 1;
    }
//    delay(250);
    analogWrite(greenLED, 255);
        for (int i = 0; i < 20000;) {
      i += 1;
    }
//    delay(500);
//    Serial.print("2");
  }

//  int iteration = 0;
//  while (1) {
//    for (int i = 0; i < 30000;) {
//      i += 1;
//    }
//    if (iteration % 2 == 0) {
//      analogWrite(greenLED, 255);
//    } else {
//      analogWrite(greenLED, green_output);
//    }
//    iteration += 1;
//  }
//  
  return;
}

void setup()
{
//  Serial.begin(9600);
  if (process_create (p1, 64) < 0) {
    return;
  } else {
//    analogWrite(blueLED, 100);
  }
//  Serial.println("Done creating p1");
  if (process_create (p2, 64) < 0) {
    return;
  } else {
//    analogWrite(greenLED, 100);
  }
//  Serial.println("Done creating p2");
}

void loop()
{
  process_start();
//  Serial.println("Processes done");
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}
