#include <SoftwareSerial.h>

int RxD=10;
int TxD=11;
char recvChar;
int i;
int a[10]={128,132,139,2,0,0,136,0,136,255};

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
   // Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    Serial.begin(57600);
    blueToothSerial.begin(57600);
    //setupBlueToothConnection();
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
       blueToothSerial.write(128);
           delay(50);
   blueToothSerial.write(132);
           delay(50);
           
  for(int i=0;i<6;i++)
       {blueToothSerial.write(a[i]);
        //blueToothSerial.print(a[i]);
           delay(50);}
           /*
           delay(10000);
         blueToothSerial.write(a[i]);
           delay(50);i++;
            blueToothSerial.write(a[i]);
           delay(50);i++;*/
            
           
           
    /*blueToothSerial.write(128);
           delay(50);
   blueToothSerial.write(132);
           delay(50);
        blueToothSerial.write(129);
        delay(50);
         blueToothSerial.write(2);
           delay(50);
             blueToothSerial.write(0);
           delay(50);
              blueToothSerial.write(0);
           delay(5000);
              blueToothSerial.write(136);
           delay(50);
              blueToothSerial.write(0);
           delay(50);*/
           
           
           
}

void loop()
{
 
     
         /* while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
       if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
    }

  if (readString.length() >0) {
      //Serial.println(readString); //see what was received
      int num=readString.toInt();
      blueToothSerial.write(num);
  }
  
          /*blueToothSerial.write(132);
          delay(50);
         blueToothSerial.write(139);
          blueToothSerial.write(2);
          blueToothSerial.write(0);
          //blueToothSerial.write(0);
          delay(2000);*/
}

