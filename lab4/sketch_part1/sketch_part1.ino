#include "concurrency.h"

void p1 (void)
{
  /* process 1 here */
  Serial.println("p1");
  return;
}

void p2 (void)
{
/* process 2 here */
  Serial.println("p1");
  return;
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
