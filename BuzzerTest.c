const int BUZZER_PIN = 12;

void setup()
{
      pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);
    //display.clearDisplay();
    //display.display();
}
