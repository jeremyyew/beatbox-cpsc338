#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used)) unsigned int testasm(unsigned char a, unsigned char b)
{
   unsigned int out;

   asm volatile(R"(

   rjmp start_of_assembly
fib:
   clr r25 ; otherwise random bits might be in the result
   ; if r24 is 0, return 
   cpi r24, 0
   breq return
   ; if r24 is 1, return 
   cpi r24, 1
   breq return
   
   ; save to stack
   push r2
   push r3 

   mov r3, r24 ; save input n 
   subi r24, 1 ; input = n-1
   rcall fib 

   mov r2, r24 ; save lower bits of 16-bit output of fib(n-1) 
   mov r24, r3 ; load input n 
   mov r3, r25 ; save higher bits 16-bit output of fib(n-1) 

   subi r24, 2 ; input = n-2
   rcall fib
   add r24, r2
   adc r25, r3 ; add 16-bit output values together
   
   ; restore from stack, in reverse order
   pop r3
   pop r2 
return:
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
   rcall fib
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