/* Modify the program from Task 5 so that the match operation uses MR1 (instead of MR3) while the match 
takes place after 15 sec. Configure the prescale register so that the Timer-Counter increments once every 
second. Display the Timer-Counter count using four LEDs connected to DIP pins 5, 6, 27 and 28. Thus, the 
LEDs should go through the binary count pattern 0000 – 0001 – 0010 - … -1111 (i.e., 1 to 15 in decimal), reset 
and start all over again. */

//same as task 5 except
//MR1 instead of MR3
//MATCH TAKES PLACE AFTER 15 SECS INSTEAD OF EVERY 10
//before it incremented at 100microsecs but now every 1 second
 

 /*red1, green, red1 + green, yellow, red1 + yellow, green + yellow, red1 + green + yellow,
 red2, red1 + red2, green + red2, red1 + green + red2, yellow + red2, red1 + yellow + red2,
 red1 + green + yellow + red2 */

#include "mbed.h"
#include "C12832_lcd.h"

C12832_LCD lcd; //C12832 lcd(p5, p7, p6, p8, p11);

DigitalOut myLED1(p8); //DIP6 red pin 8
DigitalOut myLED2(p9); //DIP5 green pin 9
DigitalOut myLED3(p10); //DIP28 yellow pin 10
DigitalOut myLED4(p11); //DIP27 red2 pin 11

void timer2_init(void)
{
    LPC_SC->PCONP |= (0x01 << 22);  // Power Timer2

    //PCLK = CCLK - setting peripheral clock
    LPC_SC->PCLKSEL1 &= ~(0x03 << 12); //clear bits 13:12
    LPC_SC->PCLKSEL1 |=  (0x01 << 12); //set CPU clock

    LPC_TIM2->TCR = 0x02; //reset
    LPC_TIM2->CTCR = 0x00; //timer mode

    uint32_t pclk = SystemCoreClock; //get real clock
    LPC_TIM2->PR = pclk - 1; //TC increments every 1 second

    LPC_TIM2->MR1 = 15; //reset every 15 seconds
    LPC_TIM2->MCR = (0x01 << 4); //reset on MR1

    LPC_TIM2->TC = 0; //clear timer counter
    LPC_TIM2->PC = 0; //clear prescale counter

    LPC_TIM2->TCR = 0x01; //start timer2
}

int main()
{
    timer2_init(); //initialise timer2

    while (1) //while connected to power
    {
        uint32_t tc = LPC_TIM2->TC & 0xF;  // lower 4 bits

        myLED1 = (tc >> 0) & 1; //bit 0 => LED1 (red1)
        myLED2 = (tc >> 1) & 1; //bit 1 => LED2 (green)
        myLED3 = (tc >> 2) & 1; //bit 2 => LED3 (yellow)
        myLED4 = (tc >> 3) & 1; //bit 3  => LED4 (red2)

        wait_ms(50); //just to run more smoothly so 50milliseconds wait
    }
}


/*
TRIAL IN BINARY (DID NOT SUCCEED):

void init_TC2 (void) 
{
   LPC_SC->PCONP |= (0x01 << 22); // (Bit 22) Power up Timer 2
   LPC_SC->PCLKSEL1 |= (0x01 << 12) ; // (Bits 13:12) Timer 2 clock rate is 96 MHz
   LPC_TIM2->TCR |= (0x01); // (Bit 0) Enable Timer-counter

   LPC_TIM2->PR = 96000000 - 1; // Timer-Counter effective clock period = 1 sec.   
   LPC_TIM2->MR1 = 15; // Match after 15 seconds
   // 96000000/96MHz = 1

   LPC_TIM2->MCR |= (0x01 << 4); // (Bit 1) Timer-Counter reset on MR3 Match
   //need 1 under 4 so [11 10 9 8  7 6 5 4  3 2 1 0] if 1 originally under 0, need to shift by 4 to be under 4
   LPC_TIM2->TC = (0x00); //Initialise Timer-Counter
}

void LED_binary (int number)
{
    //port 0, byte 1
   LPC_GPIO0->FIOPIN1 |= ( number >> 0) & 1 ;
   LPC_GPIO0->FIOPIN1 |= ( number >> 1) & 1 ;
   LPC_GPIO0->FIOPIN1 |= ( number >> 2) & 1 ;
   LPC_GPIO0->FIOPIN1 |= ( number >> 3) & 1 ;
}

int main()
{
    //port 0, byte 1
    LPC_GPIO0->FIODIR1 |= 0X01 ; //led 6 is num 34
    LPC_GPIO0->FIODIR1 |= 0X01 << 1; //led 5 is num 35
    LPC_GPIO0->FIODIR1 |= 0X01 << 2 ; //led 28 is num 34
    LPC_GPIO0->FIODIR1 |= 0X01 << 3; //led 27 is num 35

   init_TC2(); // Timer 2 initialisation function
   int value = LPC_TIM2->TC ; //naming LPC_TIM2->TC
   LED_binary (value); //putting value into the LED_binary function
   lcd.cls();
   lcd.locate(0,0);
   lcd.printf("Timer 2 Monitor");

   while(1) 
   {

       lcd.locate(0,15);
       lcd.printf("Timer2: %d", LPC_TIM2->TC);
       wait(1); //include an update delay for the LCD output of 1 sec
       //myled = !myled; //diagnostic
   }
}
*/


