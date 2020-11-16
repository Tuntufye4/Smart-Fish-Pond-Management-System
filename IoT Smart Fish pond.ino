

#include <Event.h>
#include <Timer.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

Timer t;
#include <LiquidCrystal.h>
#include<SoftwareSerial.h>

SoftwareSerial SoftSerial(2, 3);
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int i=0;
int val=0;
int turbine=13;
int motionsensor = 6;
const int analoglnPin= A1;
int sensorValue=0;
unsigned long int avgValue;
float b;
int buf[10],temp;
float salinityy= A0;
#define TRIGGER 7
#define ECHO 8
float time=0, W_level=0;
float W_temp=0;
float Fahrenheit=0;
float Phvalue=0;
float Salinity_value=0;
///static char postUrl[150];
 


 
void setup()
{

 
Serial.begin(9600);
SoftSerial.begin(115200); // Baud rate of esp8266
 
///////////////////////////////////////////////
//
// Connecting WIFI
//
SoftSerial.begin(115200); // Baud rate of esp8266
Serial.println(F("Connecting Wifi...."));
Serial.println(F("Connecting Wifi...."));
 
connect_wifi("AT",1000); // time in milliseconds
connect_wifi("AT+RST",6000); // reseting esp
connect_wifi("AT+CWMODE=1",6000); // setting esp to station mode
//connect_wifi("AT+CWQAP",6000); // setting esp to quik access point
connect_wifi("AT+CWJAP=\"yeah\",\"umaziwa1\"",6000); // Connecting wifi to router with SSID and Password.
//connect_wifi("AT+CWJAP=\"Testing\",\"12345\"",6000); // For example Wifi name= Testing &amp; Password=12345
 
delay(5000);
Serial.println(F("Wifi Connected"));
 
delay(2000);
 
t.every(3000, send2server); // 1000 milli seconds = 1 minute // timer.h class object
pinMode(TRIGGER,OUTPUT); 
 pinMode(ECHO,INPUT);
}
 
void loop()
{


 PH();

 WATER_LEVEL();

 WATER_TEMP();

 WATER_SALINITY();

delay(300);
}

void PH(){
for(int i=0; i<10; i++)  
{
buf[i]=analogRead(analoglnPin);    
delay(10);    
}
for(int i=0; i<9; i++)
{
for(int j=i+1; j<10; j++)
{
if(buf[i]>buf[j])
{
temp=buf[i];
buf[i]=buf[j];
buf[j]=temp;  
}
}
}
avgValue=0;
for(int i=2;i<8;i++)
avgValue+=buf[i];
float pHVol=(float)avgValue*5.0/1024/6;
Phvalue=-5.70*pHVol+21.34;
int speed1 = 140; 
int speed2  = 0;

if(Phvalue<7.0){ 
Serial.println("Water ph is acidic");
Serial.println(Phvalue);
delay(1000);
}

else if(Phvalue>=7.0 && Phvalue<=9.0){
 Serial.println("Water ph is (safe) for fish");
 Serial.println(Phvalue);
 delay(1000);
 }
else if(Phvalue>9.0){
 Serial.println("Water ph is alkaline");
 Serial.println(Phvalue);
 delay(1000);
 }

delay(20);
 
t.update();
}

void WATER_SALINITY(){
Salinity_value=analogRead(salinityy);
Salinity_value=map(Salinity_value,550,0,0,100);
if(Salinity_value>=50)
{
 Serial.println("salinity is high");
 Serial.println(Salinity_value);  
}
if(Salinity_value<50)
{
Serial.println("Salinity is low");   
Serial.println(Salinity_value);  
}
 delay(1000);
  
t.update();
}

void  WATER_LEVEL(){ 
digitalWrite(TRIGGER,LOW);
delayMicroseconds(2);
digitalWrite(TRIGGER,HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER,LOW);
delayMicroseconds(2);
time=pulseIn(ECHO,HIGH);
W_level=time*340/20000;
if(W_level>=8)
{

Serial.println("water level is low");
Serial.println(W_level);
delay(1000);  
    
}
else if(W_level<8)
{
Serial.println("water level is normal");
Serial.println(W_level);
delay(1000);
}
delay(20);
 
t.update();
}

void  WATER_TEMP(){
sensors.requestTemperatures();
W_temp=sensors.getTempCByIndex(0);

Serial.println(" water_temp: ");

Serial.print(W_temp); 
if(W_temp<22)
{
analogWrite(turbine, LOW);
Serial.println("water temperature is low");   
delay(1000);
 }
else if(analogRead(W_temp>=22 && W_temp<=27)){
  
analogWrite(turbine, HIGH);
Serial.println("water temperature is normal");
delay(1000);
  }

else if(W_temp>27)
{
analogWrite(turbine, LOW);
Serial.println("water temperature is high");   
delay(1000);
 }

Serial.println(sensors.getTempFByIndex(0));
 delay(1000);
  
t.update();
}


void send2server()
{
/*char Phvaluess[8];
char Salinity_valuess[8];
char W_levelss[8];
char W_tempss[8];
dtostrf(Phvalue, 5, 3, Phvaluess);
dtostrf(Salinity_value, 5, 3, Salinity_valuess);
dtostrf(W_level, 5, 3, W_levelss);
dtostrf(W_temp, 5, 3,W_tempss );
sprintf(postUrl, "/awardict/wifidata.php?Ph_level=%s&Salinity=%s&Water_level=%s=%s&Water_temp=%s",Phvaluess,Salinity_valuess,W_levelss,W_tempss);
httpGet("192.168.137.1", postUrl, 80); */


String postUrl="/awardict/wifidata.php?"; ///phValue&Water_level=W_level&Water_temp=W_temp&Salinity=salinity_value";
postUrl.concat("Ph_level=");
postUrl.concat(Phvalue);
postUrl.concat("&Salinity=");
postUrl.concat(Salinity_value);
postUrl.concat("&Water_level=");
postUrl.concat(W_level);
postUrl.concat("&Water_temp=");  
postUrl.concat(W_temp); 

Serial.println(postUrl);
httpGet("192.168.137.1", postUrl, 80); 
  
}
 
void httpGet(String ip, String path, int port)
{
 
String atHttpGetCmd = "GET "+path+" HTTP/1.0\r\n\r\n"; // GET /status.php?slot1=1&amp;slot2=0&amp;slot3=0...
//SoftSerial.println(atHttpGetCmd);
//AT+CIPSTART="TCP","192.168.20.200",80
String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\""+ip+"\","+port+""; /// establishing a TCP conection with ip and port
Serial.println(atTcpPortConnectCmd);
connect_wifi(atTcpPortConnectCmd,1000);
int len = atHttpGetCmd.length();
Serial.println(len);
String atSendCmd = "AT+CIPSEND=";
atSendCmd+=len;
connect_wifi(atSendCmd,1000); /// sending data length to server for confirmation of its length.
connect_wifi(atHttpGetCmd,1000); // sending data to server
SoftSerial.find(">"); 
SoftSerial.println(atHttpGetCmd);
SoftSerial.println(ip);

connect_wifi("AT+CIPCLOSE",6000);
}
 
void connect_wifi(String cmd, int t)
{
int temp=0,i=0;
while(1)
{
//lcd.clear();
//lcd.print(cmd);
Serial.println(cmd);
SoftSerial.println(cmd); // sending commands to esp8266 stored in cmd variable.
while(SoftSerial.available())
{
if(SoftSerial.find("OK"))
 
i=8;
}
delay(t);
if(i>5)
break;
i++;
}
if(i==8)
{
Serial.println(F("OK"));
//lcd.setCursor(0,1);
//lcd.print("OK");
}
else
{
Serial.println(F("Error"));
// lcd.setCursor(0,1);
// lcd.print("Error");
}
}
 
