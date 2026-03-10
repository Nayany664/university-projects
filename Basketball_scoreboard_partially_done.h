//need to add what happens if there are faulty plays/free throws
//happens when timer pauses

#include "mbed.h"
#include "cmsis_os.h"
#include "C12832.h"
#include "scoreboard.h"

C12832 lcd(p5, p7, p6, p8, p11);
LM75B sensor(p28,p27);

// Joystick Inputs
InterruptIn By1(p15); 
InterruptIn By2(p16); 
InterruptIn By3(p12); 
InterruptIn Team(p14); 
InterruptIn Play(p13);

// Timer used for software debouncing
Timer debounceTimer;

// Thread IDs
osThreadId score_ID, LCD_ID, time_ID, temp_ID;

// Thread Definitions
osThreadDef(score_func, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(lcd_func, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(time_func, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(temp_func, osPriorityNormal, DEFAULT_STACK_SIZE);

// Message Queue
osMessageQDef(queue, 1, uint32_t);
osMessageQId(queue_ID);

// ISR declarations
void By1_isr() {osMessagePut(queue_ID, 1, 0);} // +1 point
void By2_isr() {osMessagePut(queue_ID, 2, 0);} // +2 points
void By3_isr() {osMessagePut(queue_ID, 3, 0);} // +3 points
void Team_isr();
void Time_isr();

int main()
{
    // Start debounce timer
    debounceTimer.start();

    // Attach joystick interrupts
    By1.rise(&By1_isr);
    By2.rise(&By2_isr);
    By3.rise(&By3_isr);
    Team.rise(&Team_isr);
    Play.rise(&Time_isr);

    // Create queue
    queue_ID = osMessageCreate(osMessageQ(queue), NULL);

    // Create threads
    score_ID = osThreadCreate(osThread(score_func), NULL);
    LCD_ID = osThreadCreate(osThread(lcd_func), NULL);
    time_ID = osThreadCreate(osThread(time_func), NULL);
    temp_ID = osThreadCreate(osThread(temp_func), NULL);
}

void Team_isr()
{
    // Ignore press if it occurs within 200ms
    if(debounceTimer.read_ms() < 200)
        return;

    // Reset timer after valid press
    debounceTimer.reset();

    // Toggle selected team
    score.h0v1 = !score.h0v1;

    // Update LCD
    osSignalSet(LCD_ID, 0x2);
}

void Time_isr()
{
    score.running = !score.running;
    osSignalSet(time_ID, 0x3);
}


void Timer1_Update(void const *args)
{
    score.time_count--;

    if(score.time_count <= 0)
    {
        score.time_count = 0;
        score.running = 0;
    }

    osSignalSet(LCD_ID,0x2);
}


void score_func(void const *args)
{
    score.h0v1 = 0; 
    score.time_count = 180;
    score.home_count = 0;
    score.visitors_count = 0;

    uint32_t val;

    while(1)
    {
        osEvent score_sig = osMessageGet(queue_ID, osWaitForever);

        if(score_sig.status == osEventMessage)
        {
            val = score_sig.value.v;

            if(score.h0v1 == 0)
                score.home_count += val;
            else
                score.visitors_count += val;
        }

        osSignalSet(LCD_ID,0x2);
    }
}

void lcd_func(void const *args)
{
    while(1)
    {
        int minutes = score.time_count / 60;
        int seconds = score.time_count % 60;

        lcd.cls();
        lcd.locate(0,3);

        lcd.printf("Time remaining: %02d:%02d\n", minutes, seconds);

        if(score.h0v1 == 0)
            lcd.printf("*Home:%2d Visitors:%2d\n", score.home_count, score.visitors_count);
        else
            lcd.printf(" Home:%2d *Visitors:%2d\n", score.home_count, score.visitors_count);

        osSignalWait(0x2, osWaitForever);
    }
}


void time_func(void const *args)
{
    osTimerDef(Timer1, Timer1_Update);
    osTimerId Timer1_ID;

    Timer1_ID = osTimerCreate(osTimer(Timer1), osTimerPeriodic, NULL);

    while(1)
    {
        osSignalWait(0x3, osWaitForever);

        if(score.running == 0)
            osTimerStop(Timer1_ID);
        else
            osTimerStart(Timer1_ID,1000);
    }
}

void temp_func(void const *args)
{
    float temp;

    if(sensor.open())
    {
        while(1)
        {
            temp = sensor.temp();

            if(temp > 29.0)
            {
                lcd.cls();
                lcd.printf("Game Terminated\nTemp:%2.1f",temp);

                osThreadTerminate(score_ID);
                osThreadTerminate(time_ID);
                osThreadTerminate(LCD_ID);
            }

            osDelay(5000);
        }
    }
}
