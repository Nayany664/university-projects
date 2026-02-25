/* Task run when you move joystick */

#include "mbed.h"
#include "C12832.h"
#include "rtos.h"

C12832 lcd(p5, p7, p6, p8, p11); //LCD pins (known fact)

//the LEDs that light up for each task
DigitalOut led1(LED1); //for task 1 - output
DigitalOut led2(LED2); //for task 2 - output
DigitalOut led3(LED3); //for task 3 - output

//the pins the joystick are connected to (just up, down and left)
InterruptIn joy_up(p15); //push up for task 1
InterruptIn joy_down(p12); //push down for task 2
InterruptIn joy_left(p13); //push left for task 3

 
//time for each LED to light up in each task
const int t1_time = 3000; //task 1 LED1 lights up for 3 seconds (3000 milliseconds)
const int t2_time = 2000; //task 2 LED2 lights up for 2 seconds (2000 milliseconds)
const int t3_time = 5000; //task 3 LED3 lights up for 5 seconds (5000 milliseconds)

//used in rtos - like a counter (initially 0 threads in each semaphore)
Semaphore semaphore1(0); //
Semaphore semaphore2(0);
Semaphore semaphore3(0);


void execute_task(DigitalOut &led, int burst_time, const char* task_name)
//when joystick moved => light up the led, for however long the task should run, and output the task name
//burst time works in milliseconds that's why I did 3000, 2000 and 5000 earlier
{
    lcd.cls(); //clear the LCD screen 
    lcd.locate(0,0); //the writig should be on the (0,0) position of the LCD
    lcd.printf("Event Scheduling"); //output "Event Scheduling" at the (0,0) position
    lcd.locate(0,10); //the writig should be on the (0,10) position of the LCD so under the previous line
    lcd.printf("%s Running", task_name); //output "Running" and then the task running based on the joystick movement at the (0,10) position

    led = 1; //light up the LED for that task(1 is light up)

    wait_ms(burst_time); // wait for how long the burst time for that task is - safe delay in thread
 
    led = 0; //turn the light off the LED for that task (0 is light off)
}


void task1_thread() //what the thread should do for task 1
{
    while (true) //while task 1 is running
    {
        semaphore1.wait(); //wait for the user to move the joystick up

        execute_task(led1, t1_time, "T1"); //do task 1 based on the execute function from earlier in the code
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("T1:%d", t1_time); //print out the time running

        //after task completed, to show task completed (light turned off and screen goes back to how it was at the start)
        lcd.cls(); //clear LCD screen after task completed
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("Event Scheduling"); //print out "Event Scheduling"
    }
}

void task2_thread() //what the thread should do for task 2
{
    while (true) //while task 2 is running
    {
        semaphore2.wait(); //wait for the user to move the joystick down

        execute_task(led2, t2_time, "T2"); //do task 2 based on the execute function from earlier in the code
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("T2:%d", t2_time); //print out the time running

        //after task completed, to show task completed (light turned off and screen goes back to how it was at the start)
        lcd.cls(); //clear LCD screen after task completed
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("Event Scheduling"); //print out "Event Scheduling"
    }
}

void task3_thread() //what the thread should do for task 3
{
    while (true) //while task 3 is running
    {
        semaphore3.wait(); //wait for the user to move the joystick left

        execute_task(led3, t3_time, "T3"); //do task 3 based on the execute function from earlier in the code
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("T3:%d", t3_time); //print out the time running

        //after task completed, to show task completed (light turned off and screen goes back to how it was at the start)
        lcd.cls(); //clear LCD screen after task completed
        lcd.locate(0,0); //at (0,0) position
        lcd.printf("Event Scheduling"); //print out "Event Scheduling"
    }
}


void task1_isr() //when joystick pushed up (task 1)
{
    semaphore1.release(); //do task 1
}

void task2_isr() //when joystick pushed up (task 2)
{
    semaphore2.release(); //do task 2
}

void task3_isr() //when joystick pushed up (task 3)
{
    semaphore3.release(); //do task 3
}


int main()
{
    //at the start
    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Event Scheduling"); //ouput "Event Scheduling"

    //create each thread
    Thread t1;
    Thread t2;
    Thread t3;

    //start each thread when called upon
    t1.start(task1_thread);
    t2.start(task2_thread);
    t3.start(task3_thread);

    joy_up.rise(task1_isr); //push joystick up for task 1
    joy_down.fall(task2_isr); //push joystick down for task 2
    joy_left.rise(task3_isr); //push joystick left for task 3

    while (true) //while connected to power
    {
        wait_ms(1000); //just so it runs smoothly
    }

}

