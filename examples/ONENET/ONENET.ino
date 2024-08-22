/*****************************************************************
File:        ONENET.ino
Description: Connect to the onenet platform to upload and download data
******************************************************************/
#include "ONENET.h"
BMC81M001       Wifi(&Serial1);  //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMC81M001     Wifi(&Serial2);  //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMC81M001     Wifi(&Serial3);  //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMC81M001     Wifi(&Serial4);  //Please uncomment out this line of code if you use HW Serial4 on BMduino
String Data_String="Hello";
int Data_int=30;
float Temperature=27.5;
char pub[200];

void setup()  
{ 
  Serial.begin(9600); 
  Wifi.begin();
  Wifi.reset(); 
  Serial.print("WIFI Connection Results:");
  if(Wifi.connectToAP(WIFI_SSID,WIFI_PASS)==0) 
  {
    Serial.println("fail");
  } 
  else {Serial.println("success");}
  Serial.print("ONENET Connection Results:");
  if(Wifi.configMqtt(CLIENTLID,USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT)==0)
  {
    Serial.println("fail");
  }
  else {Serial.println("success");}
  delay(200);
  Wifi.setSubscribetopic(ONENET_TOPIC_PROP_SET);
  Wifi.setSubscribetopic(ONENET_TOPIC_PROP_GET);
  Wifi.setSubscribetopic(ONENET_TOPIC_PROP_GET_REPLY);
  Wifi.setSubscribetopic(ONENET_TOPIC_PROP_SET_REPLY);
  Wifi.setSubscribetopic(ONENET_TOPIC_PROP_POST_REPLY);
  //Single upload
    String temp=String(Temperature,1);
    SET_BUF ="{\"id\":\"";
    SET_BUF +=ID;
    ID++;
    SET_BUF +="\",";
    SET_BUF +="\"version\":\"1.0\",\"params\":";
    SET_BUF += "{\"temp\":{\"value\":"; 
    SET_BUF +=temp;
    SET_BUF +="},\"humi\":{\"value\":";
    SET_BUF +=Data_int;
    SET_BUF +="},\"String\":{\"value\":\"";
    SET_BUF +=Data_String;
    SET_BUF +="\"}}}";
    SET_BUF.toCharArray(pub,SET_BUF.length()+1);
    if(Wifi.writeBytes(pub,SET_BUF.length(),ONENET_TOPIC_PROP_POST))
      {
        Serial.println("Send String data sucess");
      }
    delay(1000);
}
void loop() 
{ 
    Wifi.readIotData(&ReciveBuff,&ReciveBufflen,&topic);
    if(Wifi.OneNetReciveBuff.length()>5)
    {
      Serial.println(Wifi.OneNetReciveBuff);
    }
    clearBuff();
    delay(1000);
}  
void clearBuff()
{
  memset(pub,'\0',200);
  resLen = 0;
  Wifi.OneNetReciveBuff="0";
}
