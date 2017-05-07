#include <SoftwareSerial.h>

int RxD=2;
int TxD=4;
char recvChar;
int i;
//int a[20]={152, 17 ,137, 1, 44, 128, 0, 156, 1, 144, 137, 1, 44, 0, 1, 157, 0, 77, 153,153};
int a[11]={152 ,17 ,137 ,1 ,44, 1 ,44 ,153,153};
String readString;
SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
   // Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    Serial.begin(38400);
    blueToothSerial.begin(38400);
    //setupBlueToothConnection();
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
    blueToothSerial.write(128);
           delay(50);
   blueToothSerial.write(132);
           delay(50);
}

void loop()
{
 
        if(blueToothSerial.available())
        {
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
        }
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
    */ 
          //delay(1000);
          for(i=0;i<11;i++)
          {blueToothSerial.write(a[i]);
           delay(50);}
          /*blueToothSerial.write(132);
          delay(50);
         blueToothSerial.write(139);
          blueToothSerial.write(2);
          blueToothSerial.write(0);
          //blueToothSerial.write(0);
          delay(2000);*/
}

