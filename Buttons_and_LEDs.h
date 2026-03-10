/* Write a program that reads in two switch (SW) settings & displays these settings with LEDs. That is, if a SW is 
in the OFF position the LED should be ON and if the SW is in the ON position the LED will be OFF. The switches 
should be connected to pins DIP17 and DIP18 and the corresponding LEDs should be LED2 and LED3 on the 
MBED board (LED2 driven by the DIP 17 SW and LED3 driven by the DIP18 SW). Although upon system reset 
all pins are configured as input by default, do include register configuration settings for configuring the switch 
pins as input.  */

//when you press switch 1 (blue), led 2 turns off, when you press switch 2 (orange), led 3 turns off

#include "mbed.h"

int main() 
{
    
    //switches
    const int switch1_PIN = 25; // DIP17 = P0.25
    const int switch2_PIN = 26; // DIP18 = P0.26

    //LEDs 
    const int LED2_PIN = 20; // LED2 = P1.20
    const int LED3_PIN = 21; // LED3 = P1.21

    LPC_GPIO0->FIODIR &= ~((1 << switch1_PIN) | (1 << switch2_PIN)); //inputs
    LPC_GPIO1->FIODIR |= (1 << LED2_PIN) | (1 << LED3_PIN);  //outputs

    while (1) //while connected to power
    {
        //if switch 0 => On
        //if switch 1 => Off
        int switch1_state = (LPC_GPIO0->FIOPIN >> switch1_PIN) & 0x1; //switch 1
        int switch2_state = (LPC_GPIO0->FIOPIN >> switch2_PIN) & 0x1; //switch 2

        if (switch1_state == 1) //if switch 1 not pressed 
        {
            LPC_GPIO1->FIOSET = (1 << LED2_PIN); //keep LED2 on
        } 
        else 
        {
            LPC_GPIO1->FIOCLR = (1 << LED2_PIN); //turn LED2 off
        }

        if (switch2_state == 1) //if switch 2 not pressed,
        {
            LPC_GPIO1->FIOSET = (1 << LED3_PIN); //keep LED3 on
        } 
        else 
        {
            LPC_GPIO1->FIOCLR = (1 << LED3_PIN); //turn LED3 off
        }
        
        wait(0.1); // delay 100ms
    }
}

