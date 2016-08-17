//#include <SoftwareSerial.h>
#include "AltSoftSerial.h"

//Wifi device -----------------------------
AltSoftSerial wifiSerial; // RX, TX 8,9

void setup() {
  // put your setup code here, to run once:
  
  wifiSerial.begin(9600);
  Serial.begin(9600 );
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
    wifiSerial.println(s);
    Serial.println(s);
  }
}
