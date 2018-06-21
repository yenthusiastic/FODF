
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Countimer.h"

#define OLED_RESET 4
/*connect OLED to Arduino:
 * SCL to A5
 * SDA to A4
 */
#define POTI_PIN A3
#define BUZZER 3 //must be a PWM pin
#define MIN_TIME 5
#define MAX_TIME 70
#define POTI_INTERVAL 5

Countimer tDown;
unsigned long previousMillis_settings = 0;        
unsigned long currentMillis_settings = 0;
const long interval_settings = 3000; 
uint8_t oldTimeout_settings = MAX_TIME;
uint8_t timeoutVal = MAX_TIME;

Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  pinMode(POTI_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  playTone();
  oldTimeout_settings = map(analogRead(POTI_PIN), 0, 1023, MIN_TIME, MAX_TIME);
  timeoutVal = oldTimeout_settings;
  // Count-down timer with current timeout value (in minutes)
  tDown.setCounter(0, 0, timeoutVal-1, tDown.COUNT_DOWN, playTone);
  // Call print_time() method every 1 minute.
  tDown.setInterval(print_time, 60000);
}
 
void loop()                     // run over and over again
{
 display.clearDisplay(); 
 showTexts();
 // display timer value on OLED
 showTime();
 display.display();
 getCurrentTimerSettings();
 tDown.run();
 tDown.start();
}


void showTexts(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.print("SET TIMER");  
  display.setCursor(60,30);
  display.setTextSize(2); 
  display.print(" min");
}

void showTime() {
  display.setCursor(10,25);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  timeoutVal = map(analogRead(POTI_PIN), 0, 1023, MIN_TIME, MAX_TIME); 
  if (timeoutVal % POTI_INTERVAL < 2){
    timeoutVal = (timeoutVal / POTI_INTERVAL) * POTI_INTERVAL;  
  } else timeoutVal = ((timeoutVal / POTI_INTERVAL) + 1) * POTI_INTERVAL;  
  display.print(timeoutVal);
}

void getCurrentTimerSettings(){
    if (timeoutVal != oldTimeout_settings){
      tDown.stop();
      currentMillis_settings = millis();
      if (currentMillis_settings - previousMillis_settings >= interval_settings) {  
          previousMillis_settings = currentMillis_settings;
          Serial.print("value changes, ");
          Serial.print("old value: ");
          Serial.print(oldTimeout_settings);
          Serial.print(", new value: ");
          Serial.println(timeoutVal);
          oldTimeout_settings = timeoutVal;
          if(timeoutVal < 60){
             tDown.setCounter(0, timeoutVal-1, 0, tDown.COUNT_DOWN, playTone);         
          } else {
            tDown.setCounter(timeoutVal/60, timeoutVal%60 - 1, 0, tDown.COUNT_DOWN, playTone);
          }       
          tDown.start();  
      }        
  }
  
}


void playTone(){
  Serial.println("times up");
  tone(BUZZER, 350, 250);
    delay(200);
    tone(BUZZER, 300, 250);
    delay(200);
    tone(BUZZER, 350, 250);
    delay(200);
}

void print_time()
{
  Serial.print("tDown: ");
  Serial.println(tDown.getCurrentTime());
}

