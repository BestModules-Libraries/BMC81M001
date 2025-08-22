/*****************************************************************
File:        ThingSpeakPublish.ino
Description: Connect ThingSpeak through MQTT parameters and send data to the specified channel
******************************************************************/
#include "ThingSpeakPublish.h"
#include <BMD31M090.h>
#include <BM25S2021-1.h>

BMC81M001   Wifi(&Serial1);  //Please uncomment out this line of code if you use HW Serial1 on BMduino
BM25S2021_1 BMht(&Wire2);
float data1;
String Humidity;
String Temperature;
void Display();

void setup() 
{ 
  Serial.begin(9600); 
  BMht.begin();                                
  Wifi.begin();
  Wifi.reset(); 
  Serial.print("WIFI Connection Results：");
  if(Wifi.connectToAP(WIFI_SSID,WIFI_PASS)==0) 
  {
    Serial.println("fail");
  } 
  else {Serial.println("success");}
  Serial.print("ThingSpeak Connection Results：");
  if(Wifi.configMqtt(CLIENTLID,USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT)==0)
  {
    Serial.println("fail");
  }
  else {Serial.println("success");}
  delay(200);
  Wifi.setSubscribetopic(SUBSCRIBERTOPIC2);
  Wifi.setSubscribetopic(SUBSCRIBERTOPIC1);
  topic = PUBLISHTOPIC;
}
void loop() 
{ 
    data1=BMht.readHumidity();
    Humidity=String(data1,2);
    DATA_BUF = "field1="; 
    DATA_BUF += Humidity; 
    if(Wifi.writeString(DATA_BUF,topic))
      {
        Serial.println("Send String data sucess");
        delay(1000);
      }
    clearBuff();
    data1=BMht.readTemperature(false);
    
    Temperature=String(data1,2);
    DATA_BUF = "field2="; 
    DATA_BUF += Temperature; 
    if(Wifi.writeString(DATA_BUF,topic))
      {
        Serial.println("Send String data sucess");
        delay(1000);
      }
      clearBuff();
    delay(2000);
 
}  
void clearBuff(){
  memset(OledBuff,'\0',RES_MAX_LENGTH);
  resLen = 0;
}
