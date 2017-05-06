#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include "RTClib.h"
#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(6,7); // RX | TX
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
RTC_DS3231 rtc;

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
int ledPin = 13;
int pirPin=3;
//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime; 
String Date; 
// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

void setup(){
  Serial.begin(38400);
  BTSerial.begin(38400);  
  delay(300);//Let system settle
  Serial.println("Humidity and temperature\n\n");
  delay(700);
    if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }

  
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
  

}

void loop(){
  sonar.ping_cm();
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

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

        }
     else 
     Serial.print("error measuring pressure");

   
   //BTSerial.println(p0*0.0295333727,2);
   
    DateTime now = rtc.now();
    
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
    
    Date = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + "  " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) ;
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
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

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
           Serial.println(" sec");
           delay(50);
           }
       }

Serial.println();
Serial.println();
}

