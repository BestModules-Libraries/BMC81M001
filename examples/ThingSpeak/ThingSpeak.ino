/*****************************************************************
File:        ThingSpeak.ino
Description: Connect ThingSpeak through MQTT parameters and send data to the specified channel
******************************************************************/
#include "ThingSpeak.h"

BMC81M001       Wifi(&Serial1);  //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMC81M001     Wifi(&Serial2);  //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMC81M001     Wifi(&Serial3);  //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMC81M001     Wifi(&Serial4);  //Please uncomment out this line of code if you use HW Serial4 on BMduino

void setup() 
{                    
  Serial.begin(115200);                                
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
}
void loop() 
{ 
    Wifi.readIotData(&ReciveBuff,&ReciveBufflen,&recTopic);             //监听模块接收的数据
    if(ReciveBufflen)
    {
        Serial.println(ReciveBufflen);
        Serial.println(ReciveBuff);
    }
      DATA_BUF = "field1="; 
      DATA_BUF += "100"; 
      topic = PUBLISHTOPIC;
    if(Wifi.writeString(DATA_BUF,topic))
    {
    Serial.println("Send String data sucess");
    }
    delay(2000);
}  
