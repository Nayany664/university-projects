#include <Servo.h> //for servo
#include <Wire.h> //to communicate with I2C device
#include <Adafruit_GFX.h> //to display on screen
#include <Adafruit_SSD1306.h> //for OLED screen

const int S0 = A1; //pins for colour sensor
const int S1 = A2;
const int S2 = A3;
const int S3 = 9;
const int OUT = 10;
//GROUND goes to GROUND

const int SERVO_PIN = 11; //pin for servo
Servo ServoMotor; //connects servo motor to servo from above so I can code it

const int BUZZER_PIN = 12; //pin for buzzer

#define SCREEN_WIDTH 128 //width of the OLED screen
#define SCREEN_HEIGHT 64 //width of the OLED screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int Red = 0; //store pulse width for each colour
int Green = 0;
int Blue = 0;
int Counternum = 0; //advanced feature

void setup() 
{
  pinMode(S0, OUTPUT); //SO and S1 control how fast the sensor replies
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); //S2 and S3 select the colour filter (red, green and blue)
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT); //OUT makes the sine wave freq into a square wave that corresponds to detected colour freq

  //S0, S1, S2 and S3 are outputs cause they control the sensor
  //OUT is an input cause this is how the freq impulses go to the Arduino

  digitalWrite(S0,HIGH); //high means the sensor outputs are at full speed
  digitalWrite(S1,LOW);

  digitalWrite(5,HIGH); //for relay to work
  delay(100);
  digitalWrite(5,LOW);

  pinMode(BUZZER_PIN, OUTPUT); //Buzzer makes sound therefore output

  Serial.begin(9600); //initialise serial communication

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //if display fails to inialise then print the below
  {
    Serial.println("SSD1306 allocation failed");
    for(;;); //infinite loop
  }

  display.clearDisplay(); //1st clear 
  display.setCursor(10, 25); //10 spaces right, 25 spaces down
  display.setTextSize(2); //size 2
  display.setTextColor(WHITE); //white writing
  display.print("Go ");
  display.display(); //print what's on display

  delay(2000); //wait 2 secs

  ServoMotor.attach(SERVO_PIN); //Servo connected to servo pin and causes armature to spin
  ServoMotor.write(90); //default position of armature and servo
}

void loop() 
{
  Red = getRedPW(); //Read Red Pulse Width
  delay(1000); //0.2 sec delay to stabalise sensor

  Green = getGreenPW(); //Read Green Pulse Width
  delay(1000); //0.2 sec delay to stabalise sensor

  Blue = getBluePW(); //Read Red Pulse Width
  delay(1000); //0.2 sec delay to stabalise sensor

  Serial.print("Red PW = "); //print freqs on serial monitor
  Serial.print(Red);
  Serial.print(" - Green PW = ");
  Serial.print(Green);
  Serial.print(" - Blue PW = ");
  Serial.print(Blue);

  display.clearDisplay(); //1st clear 
  display.setCursor(10, 10); //10 spaces right, 10 spaces down
  display.setTextSize(1); //size 1
  display.setTextColor(WHITE); //white writing
  String Color = detectColor(); //get colour detected
  display.print("Colour:  ");
  display.print(Color); //print the colour on display

  display.setCursor(10, 25); //10 spaces right, 25 spaces down
  display.setTextSize(1); //size 1
  display.setTextColor(WHITE); //white writing
  display.print("Counter number:  ");
  display.print(Counternum); //print num of counters

  display.display(); //print what's on display

  if (Color == "Purple")
  {
    ServoMotor.write(90);
    delay(2000);
    ServoMotor.write(180);

    tone(BUZZER_PIN, 1000); //buzzer buzzes at volume 1000
    delay(1000);
    noTone(BUZZER_PIN); //stop buzzing

    Counternum = Counternum + 1; //increase counter num by 1
  }

  else if (Color == "White")
  {
    ServoMotor.write(90);
    delay(2000);
    ServoMotor.write(0);

    Counternum = Counternum + 1; //increase counter num by 1
  }

  else if (Color == "Orange")
  {
    ServoMotor.write(90);
    delay(2000);
    ServoMotor.write(0);

    Counternum = Counternum + 1; //increase counter num by 1
  }

  else if (Color == "Unknown")
  {
    ServoMotor.write(90);
  }

  delay(3000); //wait 2 seconds before next reading
}

int getRedPW() //get red pulse widths
{ //sensor reads just red
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  int PW; //pulse width int
  PW = pulseIn(OUT, LOW); //pulse width output
  return PW; //return pw
}
int getGreenPW() //get green pulse widths
{ //sensor reads just green
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  int PW; //pulse width int
  PW = pulseIn(OUT, LOW); //pulse width output
  return PW; //return pw
}
int getBluePW() //get red pulse widths
{ //sensor reads just red
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  int PW; //pulse width int
  PW = pulseIn(OUT, LOW); //pulse width output
  return PW; //return pw
}
String detectColor() //these freqs I worked out
//return cause string
{
  if ((Red == 31) && (Green == 36) && (Blue == 10)) //comparison therefore need ==
  {
    return "Orange";
  }
  else if ((Red == 33) && (Green == 36) && (Blue == 10))
  {
    return "Purple";
  }
  else if ((Red == 32) && (Green == 35) && (Blue == 10))
  {
    return "White";
  }
  else
  {
    return "Unknown";
  }
}
