#include "AltSoftSerial.h"

//Wifi device -----------------------------
AltSoftSerial wifiSerial; // RX, TX 8,9

//Eranga
bool safe = true;
char lastCommand = 'p';
long duration, distance;
#define trigPin 13
#define echoPin 12

int speedValue = 255;

//Setup and Loop --------------------------------------------------------------------------------------------
unsigned long lastTime;
void setup() {

  Serial.begin(9600);
 
  setupMotors();
//  forward(255);

  //Eranga
  setupSonar();
  //Set the time
  lastTime = millis();
  
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  
  
  digitalWrite(13, HIGH);
  wifiSerial.begin(9600);
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
  setFlag();
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
    lastCommand = s[i];
    if(s[i] == 'f')
      forward(speedValue);
      
    else if(s[i] == 'b' && safe)
      backward(speedValue);

    else if(s[i] == 'B')
      backward(speedValue);
    
    else if(s[i] == 's')
      stopMotors();
    
    else if(s[i] == 'l')
      left(speedValue);
    
    else if(s[i] == 'r')
      right(speedValue);

    else if(s[i]=='1')
      speedValue = 100;

    else if(s[i]=='2')
      speedValue = 140;    
      
    else if(s[i]=='3')
      speedValue = 180;
      
    else if(s[i]=='4')
      speedValue = 220;
            
    else if(s[i]=='5')
      speedValue = 255;
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
  setMotor(1,spd,false);
  setMotor(0,spd,false);
}

void backward(unsigned int spd)
{
  setMotor(1,spd,true);
  setMotor(0,spd,true);
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

//Eranga
void setFlag(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;
  if(distance < 20){
    safe=false;
    if(lastCommand=='b')
      stopMotors();
    
  }else{
    safe=true;
  }
  //Serial.println(safe);
}

void setupSonar(){
  //Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

