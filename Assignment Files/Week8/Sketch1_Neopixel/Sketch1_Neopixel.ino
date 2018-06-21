#include <Adafruit_NeoPixel.h>
#define PIN 1 //ATTiny pin that the Neopixel (single or strip) is attached to
#define NUMPIXELS 1 //Number of LEDs in the string
#define color_mode 1 // Define the color mode
//initialize the Adafruit Neopixel object
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  switch (color_mode) {
    case 1:
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(150,0,0)); // Moderately bright blue color.
      break;
    case 2:
      pixels.setPixelColor(i, pixels.Color(0,0,150)); // Moderately bright red color. 
      break;
    case 3:
      pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
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
