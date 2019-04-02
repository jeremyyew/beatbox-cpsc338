#include "concurrency.h"

// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;
int red_output = 100;
int green_output = 100;

lock_t *lock = NULL;

void p1 (void)
{
  while (1) {
    Serial.print("p1 acquiring...\n");
    lock_acquire(lock);
    
    // Serial.print(lock->acquired);
    Serial.print("p1 acquired\n");
    analogWrite(redLED, red_output);
    delay(1000);
    analogWrite(redLED, 255);
    delay(1000);
    Serial.print("p1 end\n");
    lock_release(lock);
    Serial.print("p1 released\n");
//    delay(1000);
  }
}

void p2 (void)
{
  while (1) {
    Serial.print("p2 acquiring...\n");
    lock_acquire(lock);
    // Serial.print(lock->acquired);
    Serial.print("p2 acquired\n");
    analogWrite(greenLED, green_output);
    delay(1000);
    analogWrite(greenLED, 255);
    delay(1000);
    Serial.print("p2 end\n");
    lock_release(lock);
    Serial.print("p2 released\n");
//    delay(1000);
  }
}

// void p3 (void)
// {
//   while (1) {
//     lock_acquire(lock);
//     // Serial.print(lock->acquired);
//     Serial.print("p3 start\n");
//     analogWrite(blueLED, 100);
//     delay(1500);
//     analogWrite(blueLED, 255);
//     delay(1500);
//     Serial.print("p3 end\n");
//     lock_release(lock);
//   }
// }

void setup()
{
  Serial.begin(9600);
  lock = (lock_t *) malloc(sizeof(lock_t));
  if (lock == NULL) {
    analogWrite(blueLED, 100);
  }
  lock_init(lock);

  if (process_create (p1, 64) < 0) {
    return;
  } 
  Serial.println("Done creating p1");
  if (process_create (p2, 64) < 0) {
    return;
  } 
  Serial.println("Done creating p2");
  // if (process_create (p3, 64) < 0) {
  //   return;
  // } 
  // Serial.println("Done creating p3");
}

void loop()
{
  process_start();
  Serial.println("Processes done");
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}
