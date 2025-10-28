#include <Servo.h>

Servo myservo;

void setup() 
{
  myservo.attach(11);

}

void loop() 
{
  myservo.write(0); //tested with 0 but 60 is central for me
  delay(1000);
  myservo.write(180);
  delay(1000);

}
