
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define POTI_PIN A3
#define MIN_TIME 15
#define MAX_TIME 120
#define POTI_INTERVAL 5

Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  pinMode(POTI_PIN, INPUT);
}
 
void loop()                     // run over and over again
{
 display.clearDisplay(); 
 showTexts();
 // display timer value on OLED
 showTime();
 display.display();
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
  uint8_t timeoutVal = map(analogRead(POTI_PIN), 0, 1023, MIN_TIME, MAX_TIME); 
  if (timeoutVal % POTI_INTERVAL < 2){
    timeoutVal = (timeoutVal / POTI_INTERVAL) * POTI_INTERVAL;  
  } else timeoutVal = ((timeoutVal / POTI_INTERVAL) + 1) * POTI_INTERVAL;  
  display.print(timeoutVal);

}


