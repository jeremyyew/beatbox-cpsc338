#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

   mov r22,r24
   mov r24,r25

   push r24
   push r22
   
   rcall sumval
   
   mov r26,r24
   mov r27,r25

   pop r22
   pop r24

   rcall diffval

   add r24,r26
   adc r25,r27

   rjmp end_of_assembly

sumval:
   ; Add values in r22 and r24
   add r24,r22
   
   ; Branch if carry set
   brcs carry
   
   ; Clear r25 if no carry
   ldi r25,0
   ret

   ; Set r25 to 1 if carry
carry:
   ldi r25,1
   ret

diffval:
  ldi r25,0
  ;cp r24,r22
  cp r22,r24
  brsh a_greater_eq
  sub r24,r22
  ret

a_greater_eq:
  sub r22,r24
  mov r24,r22
  ret

;b_greater_eq:
;  sub r24,r22
;  ret

   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
