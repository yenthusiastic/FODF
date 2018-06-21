/*include libraries*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "Countimer.h"

/*CONSTANTS MACROS*/
#define BUTTON_PIN_UP 2 // the pin number of the pushbutton for clockwise direction
#define BUTTON_PIN_DOWN 3 // the pin number of the pushbutton for counter clockwise direction
#define MOSFET_PIN_UP A1  // connects to PWM A of motor controller board
#define TRANSISTOR_PIN_UP A7 //connects to ON A of motor controller board
#define MOSFET_PIN_DOWN A0 // connects to PWM B of motor controller board
#define TRANSISTOR_PIN_DOWN A6  //connects to ON B of motor controller board
#define MOTOR_TIMEOUT 35000 //the time in milliseconds after which the motor will be automatically turned off in any direction it is turning
#define NO_MOTION 0  //the idle state of the motor
#define UP_DIR 1   //state where motor goes up
#define DOWN_DIR 2  //state where motor goes down

/*connect OLED to Arduino:
 * SCL to A5
 * SDA to A4
 */
#define OLED_RESET 4
#define POTI_PIN A3
#define BUZZER 3 //must be a PWM pin
#define MIN_TIMER 15
#define MAX_TIMER 120
#define POTI_INTERVAL 5

#define NEOPIXEL_PIN 8 //the pin that the Neopixel (single or strip) is attached to
//initialize the Adafruit Neopixel object with 1 single neopixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
//initialize the OLED display object
Adafruit_SSD1306 display(OLED_RESET);

/*VARIABLES*/
uint8_t buttonState_UP;             // the current state of UP-direction button
uint8_t reading_UP = 0; //variable to store temporary UP direction button state

uint8_t buttonState_DOWN;             // the current state of DOWN-direction button
uint8_t reading_DOWN = 0; //variable to store temporary DOWN direction button state

uint8_t motor_direction = NO_MOTION; //variable to store current state of motor, initially motor is idle

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time any of the button pins was toggled
unsigned long previousMillis_State = 0; // store the last time the state of motor was toggled  

uint8_t debounceDelay = 500; // the debounce time; increase if the output flickers

Countimer tDown;
unsigned long previousMillis_settings = 0;        
unsigned long currentMillis_settings = 0;
const long interval_settings = 3000; 
uint8_t oldTimeout_settings = MAX_TIMER;
uint8_t timeoutVal = MAX_TIMER;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN_UP, INPUT);
  pinMode(MOSFET_PIN_UP, OUTPUT);
  pinMode(TRANSISTOR_PIN_UP, OUTPUT);
  digitalWrite(MOSFET_PIN_UP, 0);
  digitalWrite(TRANSISTOR_PIN_UP, 0);
  pinMode(BUTTON_PIN_DOWN, INPUT);
  pinMode(MOSFET_PIN_DOWN, OUTPUT);
  pinMode(TRANSISTOR_PIN_DOWN, OUTPUT);
  digitalWrite(MOSFET_PIN_DOWN, 0);
  digitalWrite(TRANSISTOR_PIN_DOWN, 0);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  pinMode(POTI_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  oldTimeout_settings = map(analogRead(POTI_PIN), 0, 1023, MIN_TIMER, MAX_TIMER);
  timeoutVal = oldTimeout_settings;
  // Count-down timer with current timeout value (in minutes)
  tDown.setCounter(0, 0, timeoutVal-1, tDown.COUNT_DOWN, playTone);
  // Call print_time() method every 1 minute.
  tDown.setInterval(print_time, 60000);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setPixelColor(0, pixels.Color(0,20,0)); // Set Neopixel to moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
    checkButtons();
    getMotorState();
    motorController();
    pixels.show(); 
    display.clearDisplay(); 
    showTexts();
    // display timer value on OLED
    showTime();
    display.display();
    getCurrentTimerSettings();
    tDown.run();
    tDown.start();
}


void checkButtons(){
  buttonState_UP = LOW;
  buttonState_DOWN = LOW;
  reading_UP = digitalRead(BUTTON_PIN_UP); //read momentary state of button
  reading_DOWN = digitalRead(BUTTON_PIN_DOWN); //read momentary state of button

  if (reading_DOWN) {
    if (millis() - lastDebounceTime > debounceDelay) {   
      buttonState_DOWN = true;
      lastDebounceTime = millis(); // reset the debouncing timer
    }
  }
  else if (reading_UP) {
    if (millis() - lastDebounceTime > debounceDelay) {   
      buttonState_UP = true;
      lastDebounceTime = millis(); // reset the debouncing timer
    }
  }
}


void getMotorState(){  
  if (buttonState_DOWN == HIGH) {
        if (motor_direction == DOWN_DIR){
          // if motor is going up or motor is already going down, stop motor
          motor_direction = NO_MOTION;
        } else {  
        // if motor is going up or motor is idle, make motor go down
          motor_direction = DOWN_DIR;
          previousMillis_State = millis();
        }
   } 
   else if (buttonState_UP == HIGH) {
        if (motor_direction == UP_DIR){
          // if motor is going up or motor is already going up, stop motor
          motor_direction = NO_MOTION;
         } else { 
          // if motor is going down or motor is idle, make motor go up 
          motor_direction = UP_DIR;
          previousMillis_State = millis();
         }      
    }
}


void motorController(){ 
  if (motor_direction == UP_DIR){
    //automatically turn off the motor's UP direction after 35 seconds
    if (millis() - previousMillis_State > MOTOR_TIMEOUT){
      motor_direction = NO_MOTION;
    } else {
      //else turns motor UP
      Serial.println("Motor going up");
      pixels.setPixelColor(0, pixels.Color(20,20,0));
      digitalWrite(TRANSISTOR_PIN_DOWN, 0);
      digitalWrite(MOSFET_PIN_DOWN, 0);
      digitalWrite(TRANSISTOR_PIN_UP, 1);
      digitalWrite(MOSFET_PIN_UP, 1);
    }    
  }  

  else if (motor_direction == DOWN_DIR){
    //automatically turn off the motor's DOWN direction after 35 seconds
    if (millis() - previousMillis_State > MOTOR_TIMEOUT) {
      motor_direction = NO_MOTION;
    } else {
      //else turns motor UP
      Serial.println("Motor going down");
      pixels.setPixelColor(0, pixels.Color(0,0,20));
      digitalWrite(TRANSISTOR_PIN_UP, 0);
      digitalWrite(MOSFET_PIN_UP, 0);
      digitalWrite(TRANSISTOR_PIN_DOWN, 1);
      digitalWrite(MOSFET_PIN_DOWN, 1);
    }   
    
  } else {  
    // motor direction is NO_MOTION, turn off all 4 motor controller's pins         
    Serial.println("Motor is turned off");
    pixels.setPixelColor(0, pixels.Color(0,20,0));
    digitalWrite(TRANSISTOR_PIN_DOWN, 0);
    digitalWrite(MOSFET_PIN_DOWN, 0);
    digitalWrite(TRANSISTOR_PIN_UP, 0);
    digitalWrite(MOSFET_PIN_UP, 0);
  }
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
  timeoutVal = map(analogRead(POTI_PIN), 0, 1023, MIN_TIMER, MAX_TIMER); 
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
            tDown.setCounter(timeoutVal/60, timeoutVal%60-1, 0, tDown.COUNT_DOWN, playTone);
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
