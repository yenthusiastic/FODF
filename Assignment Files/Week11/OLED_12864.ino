
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 5
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGOHEIGHT 32 
#define LOGOWIDTH  32

static const uint8_t PROGMEM snowflake_data[] = {
  0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x03,
  0xC0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x07, 0xE0, 0x00,
  0x00, 0x01, 0x90, 0x00, 0x00, 0x08, 0x94, 0x00, 0x05, 0x18,
  0x9C, 0x50, 0x1D, 0x78, 0x8F, 0x5C, 0x0F, 0x0F, 0xF8, 0x78,
  0x1B, 0x1D, 0xBC, 0xEC, 0x07, 0xDC, 0xBD, 0xF0, 0x08, 0x72,
  0xAE, 0x10, 0x04, 0x7B, 0xDE, 0x20, 0x02, 0x67, 0xF2, 0x60,
  0x03, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xE0, 0x02, 0x6F,
  0xF3, 0x60, 0x04, 0x3B, 0xCE, 0x20, 0x08, 0x72, 0xAE, 0x10,
  0x07, 0x9C, 0xBD, 0xF0, 0x1F, 0x0F, 0xB8, 0x6C, 0x0F, 0x0B,
  0xF8, 0x78, 0x1D, 0x78, 0x8F, 0x5C, 0x04, 0x18, 0x9C, 0x50,
  0x00, 0x08, 0x94, 0x00, 0x00, 0x01, 0x90, 0x00, 0x00, 0x07,
  0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xC0, 0x00,
  0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  display.clearDisplay();

  // draw a bitmap icon and 'animate' movement
  //testdrawbitmap(snowflake_data, LOGOHEIGHT, LOGOWIDTH);
  animationFull(snowflake_data, LOGOHEIGHT, LOGOWIDTH);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void animationFull(const uint8_t *bitmap, uint8_t w, uint8_t h){
  uint8_t icons[NUMFLAKES][3];
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);    
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS] - LOGOHEIGHT, bitmap, w, h, WHITE);
    }  
    display.display();
    delay(200);
    // then erase it and move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS] - LOGOHEIGHT, bitmap, w, h, BLACK);
      // move it
    icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()+ LOGOHEIGHT) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }  
    
  }
}

