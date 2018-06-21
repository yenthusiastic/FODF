#include <Adafruit_NeoPixel.h>

// Which pin on the ATTiny is connected to the NeoPixel)
#define PIXEL_PIN       1

// How many NeoPixels are attached to the ATTiny?
#define NUMPIXELS      1

// Which pin on the ATTiny is the button attached to?
#define BUTTON_PIN      2

// Number of light modes
#define NUM_CASES   7

unsigned long debounceDelay = 50;    // the debounce time for the button; increase if the output flickers
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

//Create an instance of the Neopixel strip
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int buttonState = 0; //variable to store state of the button
int light_mode = 0; //variable to store current light mode
int lastButtonState = LOW;  //variable to store the last button state
int reading = 0; //variable to read temporary button state

void setup() {
  // put your setup code here, to run once:
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(BUTTON_PIN, INPUT); 
  // set Neopixel to moderately bright green color on startup
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setPixelColor(0, pixels.Color(0,120,0)); 
  pixels.show(); // This sends the updated pixel color to the hardware.
  
}

void loop() {
  //read momentary state of button
  reading = digitalRead(BUTTON_PIN);
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      //cycle through all light modes each time the button is pressed
      if (buttonState == HIGH) {
        light_mode++;
        //reset light mode when it exceeds total number of light modes-ö
        if(light_mode > NUM_CASES){
          light_mode = 1;
        }
      }
    }
   }
      // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  //set different RGB color values for each light mode
   switch(light_mode){
    case 1:
     pixels.setPixelColor(0, pixels.Color(0,0,150)); // Moderately bright blue color. 
     break;
    case 2:
     pixels.setPixelColor(0, pixels.Color(150,0,0)); // Moderately red color.
     break;
    case 3:
     pixels.setPixelColor(0, pixels.Color(0,120,0)); // Moderately bright green color. 
     break;
    case 4:
     pixels.setPixelColor(0, pixels.Color(120,120, 0)); // Moderately bright yellow color.
     break;
    case 5:
     pixels.setPixelColor(0, pixels.Color(0,120,120)); // Moderately bright turqoise color.  
     break;
    case 6:
     pixels.setPixelColor(0, pixels.Color(120,0,120)); // Moderately bright pink color. 
     break;
    case 7:
     pixels.setPixelColor(0, pixels.Color(0,0,0)); // Turn Neopixel off.
     break;
   }
    pixels.show(); // This sends the updated pixel color to the hardware.
    
}

