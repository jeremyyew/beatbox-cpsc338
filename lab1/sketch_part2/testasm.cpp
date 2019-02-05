#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used)) unsigned int testasm(unsigned char a, unsigned char b)
{
   unsigned int out;

   asm volatile(R"(

   rjmp start_of_assembly

sumval:
   ; Add values in r22 (b) and r24 (a)
   add r24,r22
   clr r25
   adc r25, r1
   ret

diffval:
  ; Clear r25 since there will be no carry
  clr r25

  ; Compare r24 (a) and r22 (b)
  cp r24,r22

  ; Branch if r24 (a) >= r22 (b)
  brsh a_greater_eq

  ; If a < b, r22 = b - a
  ; Move result to r24 and return
  sub r22,r24
  mov r24,r22
  ret

  ; Return a-b
a_greater_eq:
  sub r24,r22
  ret

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

   ; Setup arguments, left aligned on even-numbered registers
   ; r24 = a, r22 = b
   mov r22,r25

   ; Save input elsewhere
   mov r18, r22
   mov r19, r24

   ; Call subroutine for sumval
   rcall sumval

   ; Save return value r25:r24 from sumval into r27:r26
   mov r26,r24
   mov r27,r25

   ; Restore input into r22, r24
   mov r22, r18
   mov r24, r19

   ; Uses arguments, left aligned on even-numbered registers
   rcall diffval

   ; Add result from diffval (r25:r24) to result of sumval (r27:r26)
   ; Add lower byte then add with carry for higher byte
   add r24,r26
   adc r25,r27

   rjmp end_of_assembly


   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)"
                : "=w"(out)      /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
                : "r"(a), "r"(b) /* a is %1, b is %2, both are register-sized
			  variables */
                : "r25", "r24"); /* r24, r25 are modified by this assembly
			 language fragment */

   return out;
}
