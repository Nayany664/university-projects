/* time quantum = 2 secs
do each task and keep repeating until all done
if task done, skip */

#include "mbed.h" 
#include "C12832.h"

C12832 lcd(p5, p7, p6, p8, p11); //known facts

DigitalOut led1(LED1); //for task 1 - output
DigitalOut led2(LED2); //for task 2 - output
DigitalOut led3(LED3); //for task 3 - output

const int TOTAL_TIME_SEC = 10; //total time for all tasks is 10 secs
const int TIME_QUANTUM_MS = 2000; //each task gets 3.3 seconds and if not finished within 2 seconds, finish it in the next 2 secs

int task1_remaining = 3000; //task 1 takes 3 secs
int task2_remaining = 2000; //task 2 takes 2 secs
int task3_remaining = 5000; //task 2 takes 5 secs

//initailly start at time 0 for each task 
int t1_time = 0;
int t2_time = 0;
int t3_time = 0;


void execute_task(DigitalOut &led, int &remaining_time, int &total_time, const char* task_name)
//when task happening execute
{
    if (remaining_time <= 0) //if there's still task left to finish
        return; //go back to that task (if not skip)
        
    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Round Robin"); //output "Round Robin" as that's the coding project
    lcd.locate(0,10); //at position (0,10)
    lcd.printf("%s Running", task_name); //output which task is happening at that moment

    led = 1; //light up corresponding LED

    int quantum = 0; //to track how much time used in the current time slice by that task

    while (quantum < TIME_QUANTUM_MS && remaining_time > 0) //while task time not at the end/task not completed AND under 10 secs
    {
        wait_ms(100); //wait 0.1 secs
        remaining_time -= 100; //minus task time leftover by 0.1 sec until 0
        total_time += 100; //add task time by 0.1 until task time completed
        quantum += 100; //add 0.1 sec to quantum counter
    }

    led = 0; //turn off LED
}

int main()
{
    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Round Robin"); //output "Round Robin" as that's the coding project

    Timer system_timer; //to measure total execution time
    system_timer.start(); //start timer

    while (system_timer.read() < TOTAL_TIME_SEC) //run until 2 secs up
    {
        execute_task(led1, task1_remaining, t1_time, "T1"); //do task 1
        execute_task(led2, task2_remaining, t2_time, "T2"); //do task 2
        execute_task(led3, task3_remaining, t3_time, "T3"); //do task 3

        if (task1_remaining <= 0 &&
            task2_remaining <= 0 &&
            task3_remaining <= 0)
            break; //if all tasks finished early, leave the loop (break)
    }

    lcd.cls(); //clear LCD screen
    lcd.locate(0,0); //at position (0,0)
    lcd.printf("Executed Time (ms)"); //output title "Executed Time (ms)"
    lcd.locate(0,10); //at position (0,10) 
    lcd.printf("T1:%d T2:%d", t1_time, t2_time); //output how long it took to do tasks 1 and 2
    lcd.locate(0,20); //at position (0,20) - (0,10) not log enough to fit all 3 tasks on
    lcd.printf("T3:%d", t3_time); //output how long it took to do task 3

    while (true) //while connected to power
    {

    }
}

