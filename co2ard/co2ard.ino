
#define co2PIN A2


void setup()  
{
  Serial.begin(9600);
}
void loop()      
{  Serial.println(analogRead(co2PIN));
   delay(30);
}
