#include "mbed.h"
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11);

//time variables
volatile int ms = 0;
volatile int seconds = 0;
volatile int minutes = 0;
volatile int hours = 0;

volatile int flag = 0;

void T1_isr(void);  //ISR prototype

Ticker Tick;

void T1_isr()
{
    ms++;

    if (ms >= 1000) 
    { //every 1 sec
        ms = 0;
        seconds++;

        if (seconds >= 60) 
        {
            seconds = 0;
            minutes++;

            if (minutes >= 60) 
            {
                minutes = 0;
                hours++;
            }
        }

        flag = 1; //tell main loop to update LCD
    }
}


int main()
{
    Tick.attach_us(&T1_isr, 1000); 

    lcd.cls();
    lcd.locate(0, 5);
    lcd.printf("Elapsed Time Monitor");

    char buffer[20];

    while (1) 
    {

        while (flag == 0);   // wait for new second
        flag = 0;

        // Display hh:mm:ss
        sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds); 
        //print in order hours, minutes, seconds

        lcd.locate(0, 20);
        lcd.printf("Time: %s   ", buffer);
    }
}
