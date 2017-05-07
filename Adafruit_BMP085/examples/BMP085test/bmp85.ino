
#include<SPI.h> //Load SPI Library
 
 
#include "Wire.h"    // imports the wire library for talking over I2C 
#include "Adafruit_BMP085.h"  // import the Pressure Sensor Library We are using Version one of Adafruit API for this sensor
Adafruit_BMP085 mySensor;  // create sensor object called mySensor
 
 

float pressure; //Variable for holding pressure reading
 

void setup(){
Serial.begin(9600); //turn on serial monitor
mySensor.begin();   //initialize pressure sensor mySensor
 
pinMode(10, OUTPUT); //Must declare 10 an output and reserve it

}
 
void loop() {

pressure=mySensor.readPressure(); //Read Pressure
 

Serial.print("The Pressure is: ");
Serial.print(pressure);
Serial.println(" Pa.");
Serial.println("");
delay(250); //Pause between readings.
 
}

