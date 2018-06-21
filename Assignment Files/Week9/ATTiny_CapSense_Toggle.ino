// capacitive touch toggle board's output pin 
// is connected to Digital Pin 4 on ATTiny45
#define CAPACITIVE_TOGGLE 4 
// LED is connected to Digital Pin 2 on ATTiny45
#define LED_PIN 2

void setup() {
  pinMode(CAPACITIVE_TOGGLE, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if(digitalRead(CAPACITIVE_TOGGLE)){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

