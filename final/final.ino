#include "AltSoftSerial.h"

//Wifi device -----------------------------
AltSoftSerial wifiSerial; // RX, TX 8,9


void setup() {
  // put your setup code here, to run once:
  
  pinMode(13, OUTPUT);
  
  delay(500);
  wifiSerial.begin(9600);
  delay(500);
  Serial.begin(9600 );
  delay(500);

  

  blk(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (wifiSerial.available())
  {
    String s = wifiSerial.readStringUntil('\n');
    Serial.println(s);
  }
  
  if (Serial.available())
  {
    String s = Serial.readStringUntil('\n');
    sendToWifi(s);
  }
}


//Blink this LED given times
void blk(int count)
{
  digitalWrite(13,LOW);
  delay(100);
  for(int i=0; i<count; i++)
  {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
  }
}

void sendToWifi(String str)
{
  Serial.print("SendingToWifi:");
  Serial.println(str);
  wifiSerial.println(str);
}



