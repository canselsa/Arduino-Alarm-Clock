#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int b1 = 10;   //Button1 - Switching between modes in current time
const int b2 = 9;   //Button2 - Setting the alarm ON and OFF
const int b3 = 8;  //Button3 - Switching between temperature scales (Fahrenheit /Celcius)
const int b4 = 7; //Button4 - Snooze button

int alarmPin = 13;
int tempPin = 0;

int buzzer = 0; 

int currentStateb1 = 0;
int currentStateb2 = 0;
int currentStateb3 = 0;
int currentStateb4 = 0;

int btnstateb1 = 0, btnstateb2 = 0, btnstateb3 = 0, btnstateb4 = 0;
long debounceDelay = 0.01;
long lastDebounceTimeb1 = 0;
long lastDebounceTimeb3 = 0;
long lastDebounceTimeb2 = 0;
long lastDebounceTimeb4 = 0;

int degree = false;
float Celcius, Fahrenheit;

int alarmSet = false;
int clockFormat = false;

String time;
int hour = 17;
int minute = 04;
int second = 57;
int h = 0;
int alarmHour = 17;
int alarmMin = 05;

unsigned int initialTime = millis();

void setup()
{
    cli();
    Serial.begin(9600);
    pinMode(b1, INPUT);
    pinMode(b2, INPUT);
    pinMode(b3, INPUT);
    pinMode(b4, INPUT);

    pinMode(tempPin, INPUT);
    pinMode(alarmPin, OUTPUT);

    TCNT1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = 15624;
    TIMSK1 |= (1 << OCIE1A);
    lcd.begin(16, 2);
    sei();
}

ISR(TIMER1_COMPA_vect) {
    second++;

    if (second >= 60) {
        minute++;
        second = 0;
    }
    if (minute == 60) {
        hour++;
        minute = 0;
    }
    if (hour == 24) {
        hour = 0;
    }
    if (hour == alarmHour && minute == alarmMin && alarmSet == 1) {
        tone(alarmPin, 1234);
        buzzer = true;
    }
  }

void loop()
{
    currentStateb1 = digitalRead(b1);
    int reading = analogRead(tempPin);

    //Formulas of Celcius and Fahrenheit 
    float Temperature = reading * 5.0;
    Temperature /= 1024.0;

    float Celcius = (Temperature - 0.5) * 100; //Celcius C
    float Fahrenheit = (Celcius * 9.0 / 5.0) + 32.0; //Fahrenheit F


/////////////////////////////////////////////   
// BUTTON B1 
if ((millis() - lastDebounceTimeb1) > debounceDelay) {
        if (currentStateb1 != btnstateb1) {
            btnstateb1 = currentStateb1;

          //if it is changed - to switch again
            if (btnstateb1 == 0) { 
                    if (clockFormat == false) {
                        clockFormat = true; }
                  
                   else {clockFormat = false; }
            }
        }
    }
  
/////////////////////////////////////////////  
// BUTTON B2 
    currentStateb2 = digitalRead(b2);
    if ((millis() - lastDebounceTimeb2) > debounceDelay) {
        if (currentStateb2 != btnstateb2) {
            btnstateb2 = currentStateb2;

          //switching between On/Off
            if (btnstateb2 == 0){
                    if (alarmSet == false) {
                        alarmSet = true;
                    }
                    else {
                        alarmSet = false;
                        noTone(alarmPin);
                        buzzer = false;
                }
            }
        }
    }

/////////////////////////////////////////////
// BUTTON B3 
  currentStateb3 = digitalRead(b3);
   if ((millis() - lastDebounceTimeb3) > debounceDelay) {
        if (currentStateb3 != btnstateb3) {
            btnstateb3 = currentStateb3;
          
//Temparature 
     if (btnstateb3 == 1) {
           if (degree == false) {
               degree = true;} 
                else { degree = false; }
            }
        }
    }

/////////////////////////////////////////////
// BUTTON B4
currentStateb4 = digitalRead(b4); 
    if ((millis() - lastDebounceTimeb4) > debounceDelay) {
        if (currentStateb4 != btnstateb4) {
            btnstateb4 = currentStateb4;
          
            if (btnstateb4 == 1) {
              if (buzzer == true) {
                        alarmMin=alarmMin+5; 
                        if (alarmMin >= 60) {
                            alarmHour++;
                            alarmMin = 0;
                        }
                        if (alarmHour >= 24) {
                            alarmHour = 0;
                        }
                    buzzer = false;
                    noTone(alarmPin);
                }
            }
        }
    }

        if (clockFormat == false) {
            lcd.setCursor(0, 0);
          if (hour < 10) 
            lcd.print("0"); 
            lcd.print(hour);
            lcd.print(":");
            lcd.setCursor(3, 0);
          if (minute < 10)
            lcd.print("0");
            lcd.print(minute);
            lcd.setCursor(5, 0);
            lcd.print("  ");
        }
        else if (clockFormat == true) {   
            if (hour >= 12) { 
              time = "PM";  }
          
            else { 
              time = "AM"; 
            }
   
            if (hour > 12) { 
              h = (hour - 12); 
            }
            else 
             h = hour;
             lcd.setCursor(0, 0);
          
            if (h < 10)
             lcd.print("0");
             lcd.print(h);
             lcd.print(":");
             lcd.setCursor(3, 0);
          
            if (minute < 10)
             lcd.print("0");
             lcd.print(minute);
             lcd.setCursor(5, 0);
             lcd.print(time);
        
    }

        if (alarmSet == false) {
            lcd.setCursor(0, 1);
            lcd.print("OFF Alarm "); 
            lcd.setCursor(10, 1);
            if (alarmHour < 10)lcd.print("0"); 
             lcd.print(alarmHour);
             lcd.print(":");

             lcd.setCursor(13, 1);
            
            if (alarmMin < 10)lcd.print("0"); 
             lcd.print(alarmMin);
        }
  
        else {
            lcd.setCursor(0, 1);
            lcd.print("ON  Alarm ");
            lcd.setCursor(10, 1);
            
          if (alarmHour < 10)lcd.print("0");
            lcd.print(alarmHour);
            lcd.print(":");
            lcd.setCursor(13, 1);
            
          if (alarmMin < 10)lcd.print("0");
            lcd.print(alarmMin);
        
    }

    if (degree == false) {
        lcd.setCursor(9, 0);

        if (Celcius >= 0 && Celcius < 10) {
            lcd.print("  ");
        }
        else if (Celcius >= 10 && Celcius < 100) {
            lcd.print(" ");
        }
        else if (Celcius<0 && Celcius >-10) {
            lcd.print(" ");
        }
        lcd.print(Celcius);
        lcd.print("C");

    }

    else {
        lcd.setCursor(9, 0);
        if (Fahrenheit >= 0 && Fahrenheit < 10) {
            lcd.print("  ");
        }
        else if (Fahrenheit >= 10 && Fahrenheit < 100) {
            lcd.print(" ");
        }
        else if (Fahrenheit<0 && Fahrenheit >-10) {
            lcd.print(" ");
        }
        lcd.print(Fahrenheit);
        lcd.print("F");
    }
}



