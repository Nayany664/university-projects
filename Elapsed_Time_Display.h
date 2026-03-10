/* ElapsedTimeDisplay */
/* Program to show Elapsed Time on mbed Application Board LCD Display */
/* Uses Ticker to provide the clock tick */
#include "mbed.h"
#include "C12832.h" //LCD Library
 //add C12832 library from URL http://os.mbed.com/teams/components/code/C12832/
C12832 lcd (p5, p7, p6, p8, p11); // Initialize lcd
int counter = 0, seconds = 0;
int flag = 0;
void led_switch(void); //function prototypes
void T1_isr (void);
Ticker tick; // clock tick
DigitalOut myled(LED1);

void led_switch() //function blinks led
{ 
  myled=!myled;
}
void T1_isr ()
// 1 interrupt every 10 milli sec – so after 100 interrupts increase seconds and set flag //
{
   counter++;
   if (counter >= 100) //1 second has passed?
 {
   seconds++; // Yes increment seconds
   counter = 0; // reset counter
   flag = 1; // set flag to update display
 }
}

int main()
{
  //initialise ticker with period of 10msec and attach it to T1_isr//
   tick.attach_us(&T1_isr, 10000);
   lcd.cls();
   lcd.locate(0,0);
   lcd.printf("Elapsed Time Monitor"); //print header
   while(1) 
  {
     while (flag == 0); // wait for new data
     lcd.locate(0,15);
     lcd.printf("Elapsed Seconds: %d", seconds);
     flag = 0; //reset flag
     led_switch(); //call the blink led function 
  }
}
