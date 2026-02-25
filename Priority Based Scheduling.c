/* T1 = High, T3 = Medium, T2 = Low
1 runs first, highest to lowest priority
so 1 then 3 then 2
no time slicing
run each task until finished */

#include "mbed.h" 
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11); //known facts


DigitalOut led1(LED1); //for task 1 - output - T1 - high
DigitalOut led2(LED2); //for task 2 - output - T2 - low
DigitalOut led3(LED3); //for task 3 - output - T3 - medium

const int TOTAL_TIME_SEC = 10;

int task1_remaining = 3000; //high
int task2_remaining = 2000; //low
int task3_remaining = 5000; //medium

//initailly start at time 0 for each task 
int t1_time = 0;
int t2_time = 0;
int t3_time = 0;


void execute_task(DigitalOut &led, int &remaining_time, int &total_time, const char* task_name)
//when task happening execute
{
    if (remaining_time <= 0) //if time still left for that task
        return; // skip if finished

    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Priority Scheduling"); //output "Priority Scheduling" as that's the coding project
    lcd.locate(0,10); //at position (0,10)
    lcd.printf("%s Running", task_name); //output which task is happening at that moment

    led = 1; //light up corresponding LED

    while (remaining_time > 0)  //while there is still time of a task left to finish
    {
        wait_ms(100); //wait 0.1 secs
        remaining_time -= 100; //minus task time leftover by 0.1 sec until 0
        total_time += 100; //add task time by 0.1 until task time completed
    }

    led = 0; //turn off the LED
}


int main()
{
    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Priority Scheduling"); //output "Priority Scheduling" as that's the coding project

    Timer system_timer; //to measure total execution time
    system_timer.start(); //start timer

    while (system_timer.read() < TOTAL_TIME_SEC) //run until 2 secs up
    {
        //priority order high to low
        execute_task(led1, task1_remaining, t1_time, "T1"); //do task 1 - T1 high
        execute_task(led3, task3_remaining, t3_time, "T3"); //do task 3 - T3 medium
        execute_task(led2, task2_remaining, t2_time, "T2"); //do task 2 - T2 low

        if (task1_remaining <= 0 && task2_remaining <= 0 && task3_remaining <= 0)
            break; //if all tasks finished early, leave the loop (break)
    }

    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Executed Time (ms)"); //output title "Executed Time (ms)"

    lcd.locate(0,10); //at position (0,10)
    lcd.printf("T1:%d T2:%d", t1_time, t2_time);  //output how long it took to do tasks 1 and 2

    lcd.locate(0,20); //at position (0,20)
    lcd.printf("T3:%d", t3_time);  //output how long it took to do task 3

    while (true) //while connected to power
    {

    }
}
