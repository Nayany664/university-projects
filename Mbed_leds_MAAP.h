/*Write a program that flashes in order (following some user-defined delay) all four of the onboard LEDs (pins 
LED1 – LED4 on the MBED board). Your program should demonstrate the use of hardware control registers, 
i.e., it should drive the LEDs ON and OFF without using the MBED APIs/libraries. To implement the delay, you 
can simply use the MBED Wait() function set for example to produce a delay of 1 second.*/

//just on the mbed, the 4 leds should light up one after the other with 1 second gap of light staying on

#include "mbed.h"

//Port 1 byte 2
int main() 
{
    LPC_GPIO1->FIODIR2 |= 0X01 <<2; //led 1 is num 32
    LPC_GPIO1->FIODIR2 |= 0X01 <<4; //led 2 is num 34
    LPC_GPIO1->FIODIR2 |= 0X01 <<5; //led 3 is num 35
    LPC_GPIO1->FIODIR2 |= 0X01 <<7; //led 4 is num 37

  while(1) //while power conected
  
  {
      LPC_GPIO1->FIOPIN2 = 0X04; //led 1 lights up whilst rest off
      wait(1); //wait a second

      LPC_GPIO1->FIOPIN2= 0X10; //led 2 lights up whilst rest off
      wait(1); //wait a second

      LPC_GPIO1->FIOPIN2 = 0X20; //led 3 lights up whilst rest off
      wait(1); //wait a second

      LPC_GPIO1->FIOPIN2= 0X80; //led 4 lights up whilst rest off
      wait(1); //wait a second
  }
}
