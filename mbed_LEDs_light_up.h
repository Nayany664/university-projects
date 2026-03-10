#include "mbed.h"

volatile int counter = 0; //to know which LED it is currently on
volatile char drive = 0x08; //start from 0x08 to go backwards
void T1_isr (void);

Ticker Step; //timeout does
BusOut leds(LED1, LED2, LED3, LED4);

void T1_isr ()
{
    counter++;

    if (counter >= 2000) //update every 2 secs
    {
        if (drive == 0x01) //reverse of old sequence
            drive = 0x08; //8 4 2 1
        else 
            drive >>= 1; //go binary right

        counter = 0; //go back to 0
        leds = drive;
    }
}

int main()
{
    Step.attach_us(&T1_isr, 1000);
    
    leds = drive;
    while (1) 
    {
        //loop until no power
    }
}
