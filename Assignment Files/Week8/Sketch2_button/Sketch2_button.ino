/*
  Debounce

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the output pin is toggled from LOW to HIGH or HIGH to LOW. There's a
  minimum delay between toggles to debounce the circuit (i.e. to ignore noise).

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.

  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Debounce
*/
#include <SoftwareSerial.h>
#define BUTTON_PIN 2    // the pin number of the pushbutton
#define NUM_CASES 7 //the number of color modes
#define RX    5   // *** D5, Pin 1
#define TX    0   // *** D0, Pin 5
SoftwareSerial Serial(RX, TX);
int light_mode = 0; //variable to store current light mode
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int reading = 0; //variable to read temporary button state
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);}
void loop() {
  reading = digitalRead(BUTTON_PIN); //read momentary state of button
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {   
    lastDebounceTime = millis(); // reset the debouncing timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (reading != buttonState) { 
      buttonState = reading;
      //cycle through all light modes each time the button is pressed
      if (buttonState == HIGH) {
        light_mode++;
        //reset light mode when it exceeds total number of light modes
        if(light_mode > NUM_CASES){
          light_mode = 1;
        }Serial.println(light_mode);}}}   
  lastButtonState = reading; // save the reading. Next time through the loop, it'll be the lastButtonState:
  }
