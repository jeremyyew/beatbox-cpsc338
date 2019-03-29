#include "concurrency.h"

void p1 (void)
{
  /* process 1 here */
  return;
}

void p2 (void)
{
/* process 2 here */
 return;
}

void setup()
{
  if (process_create (p1, 64) < 0) {
    return;
  }
  if (process_create (p2, 64) < 0) {
    return;
  }
}

void loop()
{
  process_start();
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}
