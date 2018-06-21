/*CONSTANTS MACROS*/
#define BUTTON_PIN_CW 2 // the pin number of the pushbutton for clockwise direction
#define MOSFET_PIN_CW 9  // connects to PWM A of motor controller board
#define TRANSISTOR_PIN_CW 7 //connects to ON A of motor controller board
#define BUTTON_PIN_CCW 3 // the pin number of the pushbutton for counter clockwise direction
#define MOSFET_PIN_CCW 6 // connects to PWM B of motor controller board
#define TRANSISTOR_PIN_CCW 8  //connects to ON B of motor controller board
#define MAX_TIME 35000 //the time in milliseconds after which the motor will be 
                      //automatically turned off in any direction it is turning
#define NO_MOTION 0  //the idle state of the motor
#define CW_DIR 1   //clockwise rotation state of motor
#define CCW_DIR 2  //counter clockwise rotation state of motor
/*VARIABLES*/
uint8_t currentState_CW = 0; //variable to store current state of push button for CW direction
uint8_t buttonState_CW;             // the current reading of CW direction button
uint8_t lastButtonState_CW = LOW;   // the previous reading of CW direction button
uint8_t reading_CW = 0; //variable to store temporary CW direction button state

uint8_t currentState_CCW = 0; //variable to store current state of push button for CCW direction
uint8_t buttonState_CCW;             // the current reading of CCW direction button
uint8_t lastButtonState_CCW = LOW;   // the previous reading of CCW direction button
uint8_t reading_CCW = 0; //variable to store temporary CCW direction button state

uint8_t motor_direction = NO_MOTION; //variable to store current state of motor, initially motor is idle
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime_CW = 0;  // the last time the CW direction button pin was toggled
unsigned long previousMillis_CW = 0; // store the last time the CW direction button pin was toggled   
unsigned long lastDebounceTime_CCW = 0;  // the last time the CCW direction button pin was toggled
unsigned long previousMillis_CCW = 0; // store the last time the CCW direction button pin was toggled        
unsigned long currentMillis_Timer = 0; // store the current time for the CCW direction button

uint8_t debounceDelay = 50; // the debounce time; increase if the output flickers


void setup() {
  pinMode(BUTTON_PIN_CW, INPUT);
  pinMode(MOSFET_PIN_CW, OUTPUT);
  pinMode(TRANSISTOR_PIN_CW, OUTPUT);
  digitalWrite(MOSFET_PIN_CW, 0);
  digitalWrite(TRANSISTOR_PIN_CW, 0);
  pinMode(BUTTON_PIN_CCW, INPUT);
  pinMode(MOSFET_PIN_CCW, OUTPUT);
  pinMode(TRANSISTOR_PIN_CCW, OUTPUT);
  digitalWrite(MOSFET_PIN_CCW, 0);
  digitalWrite(TRANSISTOR_PIN_CCW, 0);
  Serial.begin(9600);
}
void loop() {
  checkButtons();
  getMotorState();
  motorController();
 }

void checkButtons(){
  reading_CW = digitalRead(BUTTON_PIN_CW); //read momentary state of button
  reading_CCW = digitalRead(BUTTON_PIN_CCW); //read momentary state of button
  // If the switch changed, due to noise or pressing:
  if (reading_CW != lastButtonState_CW) {   
    lastDebounceTime_CW = millis(); // reset the debouncing timer
  }
  if (reading_CCW != lastButtonState_CCW) {   
    lastDebounceTime_CCW = millis(); // reset the debouncing timer
  }
  if ((millis() - lastDebounceTime_CW) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (reading_CW != buttonState_CW) { 
      buttonState_CW = reading_CW;
      previousMillis_CW = millis();
      }}   
  if ((millis() - lastDebounceTime_CCW) > debounceDelay) {
  // whatever the reading is at, it's been there for longer than the debounce
  // delay, so take it as the actual current state:
    if (reading_CCW != buttonState_CCW) { 
      buttonState_CCW = reading_CCW;
      previousMillis_CCW = millis();
    }}
  lastButtonState_CW = reading_CW; // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState_CCW = reading_CCW;
}

void getMotorState(){
  currentMillis_Timer = millis();  
  if (buttonState_CCW == HIGH) {
        if (motor_direction == CCW_DIR){
          motor_direction = NO_MOTION;
          Serial.println("here 1");
        } else if (motor_direction == CW_DIR || motor_direction == NO_MOTION){
          motor_direction = CCW_DIR;
          previousMillis_CCW = millis();
          Serial.println("here 2");
        }
   } 
   if (buttonState_CW == HIGH) {
        if (motor_direction == CW_DIR){
          motor_direction = NO_MOTION;
          Serial.println("here 1");
         } else if (motor_direction == CCW_DIR || motor_direction == NO_MOTION) { 
          motor_direction = CW_DIR;
          previousMillis_CW = millis(); 
          Serial.println("here 2");
         }      
    }
}
void motorController(){ 
  if (motor_direction == CW_DIR){
    //automatically turn off the motor's CW direction after 35 seconds
    if (currentMillis_Timer - previousMillis_CW > MAX_TIME){
      motor_direction = NO_MOTION;
      previousMillis_CW = currentMillis_Timer;
    } else {
      //else turns motor Clockwise
      Serial.print("motor direction: ");
      Serial.print(motor_direction);
      Serial.println(", Clockwise direction on");
      digitalWrite(TRANSISTOR_PIN_CCW, 0);
      digitalWrite(MOSFET_PIN_CCW, 0);
      digitalWrite(TRANSISTOR_PIN_CW, 1);
      digitalWrite(MOSFET_PIN_CW, 1);
    }    
  }  

  else if (motor_direction == CCW_DIR){
    //automatically turn off the motor's CCW direction after 35 seconds
    if (currentMillis_Timer - previousMillis_CCW > MAX_TIME) {
      motor_direction = NO_MOTION;
      previousMillis_CCW = currentMillis_Timer;
    } else {
      //else turns motor Counter Clockwise
      Serial.print("motor direction: ");
      Serial.print(motor_direction);
      Serial.println(", Counter clockwise direction on");
      digitalWrite(TRANSISTOR_PIN_CW, 0);
      digitalWrite(MOSFET_PIN_CW, 0);
      digitalWrite(TRANSISTOR_PIN_CCW, 1);
      digitalWrite(MOSFET_PIN_CCW, 1);
    }   
    
  } else if (motor_direction == NO_MOTION){
    Serial.print("motor direction: ");
    Serial.print(motor_direction);
    Serial.println(", CW & CCW direction off");
    digitalWrite(TRANSISTOR_PIN_CCW, 0);
    digitalWrite(MOSFET_PIN_CCW, 0);
    digitalWrite(TRANSISTOR_PIN_CW, 0);
    digitalWrite(MOSFET_PIN_CW, 0);
  }
}

