#include <SoftwareSerial.h>
// *** Define the RX and TX pins. Choose any two
// *** pins that are unused. Try to avoid D0 (pin 5)
// *** and D2 (pin 7) if you plan to use I2C.
#define RX    1   // *** D1, Pin 6
#define TX    0   // *** D0, Pin 5
#define VCC 5.0
SoftwareSerial Serial(RX, TX);
//TMP36 Pin Variables
int sensorPin = A2; // *** D4/ A2, Pin 3 the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //400 mV offset to allow for negative temperatures
int reading = 0;
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
}
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 reading = analogRead(sensorPin);  
 Serial.print("reading: "); Serial.println(reading);
 float voltage = reading * VCC;
 voltage /= 1024.0; 
 // print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 // now print out the temperature
 float temperatureC = (voltage - 0.4) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                               //to degrees ((voltage - 400mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 delay(1000);                                     //waiting a second
}
