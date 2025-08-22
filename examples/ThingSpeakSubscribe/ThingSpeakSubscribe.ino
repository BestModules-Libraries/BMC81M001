/*****************************************************************
File:        ThingSpeakSubscribe.ino
Description: Connect ThingSpeak through MQTT parameters and send data to the specified channel
******************************************************************/
#include "ThingSpeakSubscribe.h"
#include <BMD31M090.h>
#include <BM25S2021-1.h>

BMC81M001       Wifi(&Serial1);  //Please uncomment out this line of code if you use HW Serial1 on BMduino
BMD31M090     BMD31(128,64, &Wire2); //Please uncomment out this line of code if you use HW Wire on BMduino
float data1;
String Humidity;
String Temperature;
void Display();

void setup() 
{
  BMD31.begin(0x3C);
  BMD31.setFont(FontTable_8X16);
  delay(100);     
  Serial.begin(9600);                                
  Wifi.begin();
  Wifi.reset(); 
  Display();
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
    
}
void loop() 
{ 
        Wifi.readIotData(&ReciveBuff,&ReciveBufflen,&topic);             //监听模块接收的数据
        if(ReciveBufflen)
        {
          if(topic==SUBSCRIBERTOPIC1)
          {
          ReciveBuff.toCharArray(OledBuff,7);
          BMD31.drawString(40,2,(u8*)OledBuff);
          BMD31.drawString(85,2,(u8*)"%");
          ReciveBufflen=0;
          }
          if(topic==SUBSCRIBERTOPIC2)
          {
          ReciveBuff.toCharArray(OledBuff,7);
          BMD31.drawString(40,6,(u8*)OledBuff);
          BMD31.drawString(85,6,(u8*)"C");
          ReciveBufflen=0;
          }
        }
    clearBuff();
}  
void clearBuff(){
  memset(OledBuff,'\0',RES_MAX_LENGTH);
  resLen = 0;
}
void Display()
{
  BMD31.display();
  BMD31.drawString(5,0,(u8*)"Humidity:");
  BMD31.drawString(5,4,(u8*)"Temperature:");
}
