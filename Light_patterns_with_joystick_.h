#include "mbed.h"
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11);

BusOut stepper(LED1, LED2, LED3, LED4);
Ticker Step;

//controlling motor
volatile int direction = +1; //+1 clockwise, -1 anticlockwise
volatile float step_period = 2.0; //default update every 2 seconds
volatile int step_index = 0; //current position in sequence (change back to this)

char sequence[4] = {0x01, 0x02, 0x04, 0x08};

Ticker Tick;

volatile int ms = 0;
volatile int seconds = 0;
volatile int minutes = 0;
volatile int hours = 0;

volatile int rtc_update_flag = 0; //tells main loop to update LCD

//based on info given
InterruptIn js_left(p13); //left anticlockwise
InterruptIn js_right(p16); //right clockwise
InterruptIn js_up(p15); //up slow down
InterruptIn js_down(p12); //down speed up
InterruptIn js_center(p14); //press centre reverse 

void stepper_isr()
{
    stepper = sequence[step_index]; //output pattern
    step_index += direction; //next direction

    if (step_index >= 4) step_index = 0; //4 is last one
    if (step_index < 0)  step_index = 3; // 0 1 2 3
}

void rtc_isr()
{
    ms++;

    if (ms >= 1000)
    {
        ms = 0;
        seconds++;

        if (seconds == 60)
        {
            seconds = 0;
            minutes++;

            if (minutes == 60)
            {
                minutes = 0;
                hours++;
            }
        }

        rtc_update_flag = 1;
    }
}

void isr_left() //left anticlockwise
{
    direction = -1;
}

void isr_right() //right clockwise
{
    direction = +1;
}

void isr_up()
{
    step_period = 3.0;
    Step.attach(&stepper_isr, step_period); //reconfigure ticker become 0
}

void isr_down()
{
    step_period = 1.0;
    Step.attach(&stepper_isr, step_period); //reconfigure ticker become 0
}

void isr_center()
{
    direction = -direction; //other way round sign
}

int main()
{
    Step.attach(&stepper_isr, step_period);

    Tick.attach_us(&rtc_isr, 1000);

    //attach joystick interrupts with correct trigger edges
    js_left.rise(&isr_left); //left anticlockwise
    js_right.rise(&isr_right); //right clockwise
    js_up.rise(&isr_up); //up slow down
    js_down.fall(&isr_down); //down speed up
    js_center.fall(&isr_center); //centre reverse of current

    // Initial LCD text
    lcd.cls();
    lcd.locate(0, 3); //position of writing
    lcd.printf("Stepper + Real Time Clock");

    char buffer[20]; //pos

    while (1)
    {
        if (rtc_update_flag)
        {
            rtc_update_flag = 0;

            sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds); 
            //order to print out

            lcd.locate(0, 20);
            lcd.printf("Time: %s   ", buffer); //output and keep in buffer
        }
    }
}

