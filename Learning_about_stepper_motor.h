#include "mbed.h"
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11);

BusOut stepper(LED1, LED2, LED3, LED4);    
Ticker Step;                                

volatile int step_index = 0;                
char sequence[4] = {0x01, 0x02, 0x04, 0x08}; //4-step sequence

Ticker Tick;

volatile int ms = 0;     
volatile int seconds = 0;
volatile int minutes = 0;
volatile int hours = 0;

volatile int update_LCD = 0; // tells main loop to update LCD

void stepper_isr() 
{
    stepper = sequence[step_index]; //output next step pattern to the LEDs

    step_index++;  //move to next pattern clockwise
    if (step_index >= 4)
        step_index = 0; // back to first pattern
}

void timer_isr() 
{
    ms++;

    if (ms >= 1000) //1 sec
    {
        ms = 0;
        seconds++; //add to seconds

        if (seconds == 60) 
        {
            seconds = 0;
            minutes++; //add to mins

            if (minutes == 60) 
            {
                minutes = 0;
                hours++; //add to hours
            }
        }

        update_LCD = 1; //refresh LCD
    }
}

int main() 
{
    Step.attach(&stepper_isr, 1.0); //stepper updates once per second
    Tick.attach_us(&timer_isr, 1000); // RTC updates every 1 ms

    lcd.cls();
    lcd.locate(0, 5);
    lcd.printf("Time + Stepper Motor");

    char buffer[20];

    while (1) 
    {
        if (update_LCD == 1) //update LCD once every second
        {

            update_LCD = 0; //acknowledge update request

            sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds); 
            //print in order hours, minutes, seconds

            lcd.locate(0, 20);
            lcd.printf("Time: %s   ", buffer);
        }
    }
}


