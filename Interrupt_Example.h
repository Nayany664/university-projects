//Simple interrupt example. External input causes interrupt, while led flashes //
#include "mbed.h"
InterruptIn button(p5); //define and name the interrupt input
DigitalOut led1(LED1);
DigitalOut led4(LED4);
void ISR1 () 
{ //this is the response to interrupt
 led1=!led1;
}
int main() 
{
 button.rise(&ISR1); // attach the address of the ISR function to the interrupt rising edge
 while(1) 
  { // continuous loop, ready to be interrupted
     led4=!led4;
     wait (0.25);
  }
}
