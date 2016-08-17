#include "AltSoftSerial.h"

//Wifi device -----------------------------
AltSoftSerial wifiSerial; // RX, TX 8,9

//Eranga
bool safe = true;


//Setup and Loop --------------------------------------------------------------------------------------------
unsigned long lastTime;
void setup() {

  setupMotors();
//  forward(255);

  //Set the time
  lastTime = millis();
  
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  
  
  digitalWrite(13, HIGH);
  wifiSerial.begin(9600);
  Serial.begin(9600);
  delay(500);
  digitalWrite(13, LOW);
  
  
  //Setting up
  Serial.println("Me:Setting up");

  //Set mode for both clien and server mode
  sendToWifi("AT+CWMODE=3");
  
  //connect
  Serial.println("Me:Tring to connect");
  //Connect to hostpot
  sendToWifi("AT+CWJAP=\"HTC_D626q\",\"rangerqwe\"");

  delay(6000);
 // Serial.println("Enabling Multi Channel");
  //Enable multi channel
  sendToWifi("AT+CIPMUX=1");
  //Get IP
 // Serial.println("Getting IP");
  sendToWifi("AT+CIFSR");
  //Start server

  
 // Serial.println("Starting Serverss");
  sendToWifi("AT+CIPSERVER=1,1336");

  setupMotors();
  blk(5); 
}

//-----------------------------------------------------------------------------------Loop
void loop() {
  // put your main code here, to run repeatedly: 
  if (wifiSerial.available())
  {
    String s = wifiSerial.readStringUntil('\n');
    String extracted  = extract(s);
    if(extracted=="")
      Serial.println(s);
    else
    {
      Serial.print("Original:"+s+" | ");
      Serial.println("Extracted:"+extracted);
    }
    process(extracted);
  }
  if (Serial.available())
  {
    String s = Serial.readStringUntil('\n');
    process(s);
    wifiSerial.println(s);
    Serial.println(s);
  }

  //Eranga

  
}

void blk(int count)
{
  digitalWrite(13,LOW);
  delay(100);
  for(int i=0; i<count; i++)
  {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(50);
  }
}


void sendToWifi(String str)
{
  sendToWifi(str,2000);
}

void sendToWifi(String str, int t)
{
  delay(t);
  while(wifiSerial.available())
  {
    Serial.println(wifiSerial.readStringUntil('\n'));
  }
  wifiSerial.println(str);
  blk(2);
  delay(t);
  while(wifiSerial.available())
  {
    Serial.println(wifiSerial.readStringUntil('\n'));
  }
}

void sendToClient(String str, int chn)
{
  wifiSerial.println("AT+CIPSEND="+(String)chn+","+(String)str.length());
  delay(100);
  wifiSerial.println(str);
}


String extract(String s)
{
  if(!(s[0]=='+' && s[1]=='I' && s[2]=='P' && s[3]=='D')) return "";
  for(int i=0; i<s.length(); i++)
  {
    if(s[i]==':')
    {
      String msg = s.substring(i+1);
      return msg;
    }
  }
  return "";
}

void process(String s)
{
  if(s=="") return;
  for(int i=0; i<s.length(); i++)
  {
    if(s[i] == 'f')
      forward(255);
      
    if(s[i] == 'b')
      backward(255);
    
    if(s[i] == 's')
      stopMotors();
    
    if(s[i] == 'l')
      left(255);
    
    if(s[i] == 'r')
      right(255);
      
  }
}



//---------------------------------------------------------------- Motor Control

//---------------------------------------------------------------- Motor Control
unsigned int pins[] = {5,6,7,3,2,4}; // pwm, others
void setupMotors()
{
  for(int i=0; i<6; i++)
  {
    pinMode(pins[i], OUTPUT);
  }
}

void setMotor(unsigned int motor, unsigned int spd, bool dir)
{
  unsigned int shift = 3*motor;
  analogWrite(pins[shift], spd);

  if(dir)
  {
    digitalWrite(pins[1+shift], HIGH);
    digitalWrite(pins[2+shift], LOW);
  }
  else
  {
    digitalWrite(pins[1+shift], LOW);
    digitalWrite(pins[2+shift], HIGH);
  }
}

void stopMotors()
{
  digitalWrite(pins[1],HIGH) ;
  digitalWrite(pins[2],HIGH) ;
  digitalWrite(pins[4],HIGH) ;
  digitalWrite(pins[5],HIGH) ;
}

// Other functions
void forward(unsigned int spd)
{
  setMotor(0,spd,true);
  setMotor(1,spd,true);
}

void backward(unsigned int spd)
{
  setMotor(0,spd,false);
  setMotor(1,spd,false);
}

void left(unsigned int spd)
{
  setMotor(0,spd,false);
  setMotor(1,spd,true);
}

void right(unsigned int spd)
{
  setMotor(0,spd,true);
  setMotor(1,spd,false);
}


