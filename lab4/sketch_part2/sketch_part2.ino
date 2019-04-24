#include "concurrency.h"
/*
In this example, we use the same blinking pattern in each individual process as in part 1. However, we implement mutual exclusion over the code run in each process. This means that we treat the RGB LED as a shared resource. If one process acquires the lock, the other process cannot write to the LED. Therefore, we do not expect to see the LED display two colors at any point in time. Specifically, the expected pattern is: 
  RX -> XX -> XG -> XX 
where X means no light for that pin, with red blinking for 500ms and blinking out for 500ms before green comes on for 1000ms and blinks out for 1000ms. 
*/

// RGB pins mapped to digital pins 11, 10, 9 respectively
int redLED = 11;
int greenLED = 10;
int blueLED = 9;

lock_t *lock = NULL;

void p1(void)
{
  while (1)
  {
    Serial.print("p1 acquiring...\n");
    lock_acquire(lock);
    Serial.print("p1 acquired\n");
    analogWrite(redLED, 100);
    delay(500);
    analogWrite(redLED, 255);
    delay(500);
    Serial.print("p1 end\n");
    lock_release(lock);
    Serial.print("p1 released\n");
  }
}

void p2(void)
{
  while (1)
  {
    Serial.print("p2 acquiring...\n");
    lock_acquire(lock);
    Serial.print("p2 acquired\n");
    analogWrite(greenLED, 100);
    delay(1000);
    analogWrite(greenLED, 255);
    delay(1000);
    Serial.print("p2 end\n");
    lock_release(lock);
    Serial.print("p2 released\n");
  }
}

void setup()
{
  Serial.begin(9600);
  lock = (lock_t *)malloc(sizeof(lock_t));
  if (lock == NULL)
  {
    analogWrite(blueLED, 100); // Check if malloc succeeded
  }
  lock_init(lock);

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
