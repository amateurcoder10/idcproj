#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include "RTClib.h"
#include <NewPing.h>
#include <SparkFun_ADXL345.h>      

#define TRIGGER_PIN  8
#define ECHO_PIN     4
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(6,7); // RX | TX
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
RTC_DS3231 rtc;
ADXL345 adxl = ADXL345();  
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define ALTITUDE 920.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters


#define dhtpin A0 //no ; here. Set equal to channel sensor is on
#define ldrpin A1
#define co2pin A2

int ldr=0;
int co2=0;
dht DHT;
char status;
double T,P,p0,a;
int calibrationTime = 30;        
int ledPin = 9;
int pirPin=3;
int accPin=13;
//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean var=false;
boolean mvt=true;
boolean takeLowTime; 
String Date; 
// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

void setup(){
  Serial.begin(38400);
  BTSerial.begin(38400); 
     Serial.println("Welcome to the Warehouse monitoring system\n");
   Serial.println("Setting up all the sensors");
  delay(300);//Let system settle
  Serial.println("Humidity and temperature");
  delay(700);
    if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); }// Pause forever.*/
     adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(70);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(25);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(5);         // How many seconds of no activity is inactive?3
 

  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(0, 0, 0, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt

  

  
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
   pinMode(pirPin, INPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(accPin, OUTPUT);
   digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating pir sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println(" PIR SENSOR ACTIVE");
    delay(50);
  Serial.println("setup done.beginning transmission of sensor data");

}

void loop(){
  sonar.ping_cm();
    int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial
  /* UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES */  
  /*Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z); 
  */
  ADXL_ISR();
  Serial.print("activty of the robot");
  Serial.println(mvt);
  DHT.read11(dhtpin);
  ldr = analogRead(ldrpin);
  co2 = analogRead(co2pin);
    Serial.print("co2 level=");
    Serial.print(co2);
    Serial.print("humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.print("C  ");
    Serial.print("light=");
    Serial.println(ldr);//0-1023
    
    
  delay(800);
  
   status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
        }
     else 
     Serial.print("error measuring pressure");

   
   //BTSerial.println(p0*0.0295333727,2);
   
  DateTime now = rtc.now();
    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    */
   Date = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + "  " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) ;
    //Serial.println(Date);
    BTSerial.println(Date);
    BTSerial.println(DHT.humidity);
    BTSerial.println(DHT.temperature);
    BTSerial.println(P*0.0295333727,2); 
    BTSerial.println(co2);
    BTSerial.println(ldr);
    Serial.println("Date & Time is: " + Date);
    //BTSerial.println(now.minute() + "/" + now.second());
  
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         var=true;
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
            digitalWrite(ledPin, LOW); 
            var=false;
           Serial.println(" sec");
           delay(50);
           }
       }
       BTSerial.println(var);
     
  BTSerial.println(mvt);

Serial.println();
Serial.println();
}
void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource(); 
  Serial.println("***********************************************");
  // Inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    Serial.println("*** INACTIVITY ***");
     //add code here to do when inactivity is sensed
     mvt=false;
     digitalWrite(accPin,LOW);
  }
  /*else
  Serial.println("nopeeeeeeeeeeeeeeeeeeeee");*/
  
  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("*** ACTIVITY ***"); 
     //add code here to do when activity is sensed
     mvt=true;
      digitalWrite(accPin,HIGH);
  }
  /*else
  Serial.println("nopeeeeeeeeeeeeeeeeeeeee");*/
}
