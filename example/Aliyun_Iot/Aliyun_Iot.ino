/*****************************************************************
File:        Aliyun_Iot.ino
Description: 1. Hardware serial port 1 is used for communication with BMC81M001, with a communication rate of 115200bps
             2. The serial port is used as a monitoring serial port to print and output some status of BMC81M001
             3. Automatically complete BMC81M001 networking action when powered on
             4. When the serial port sends data, its data is directly transmitted to the ALIYUN IOT platform through transparent transmission
******************************************************************/
#include "Aliyun_Iot.h"

BMC81M001       Wifi(&Serial1);  //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMC81M001     Wifi(&Serial2);  //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMC81M001     Wifi(&Serial3);  //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMC81M001     Wifi(&Serial4);  //Please uncomment out this line of code if you use HW Serial4 on BMduino

void setup() 
{
  digitalWrite(LED, LOW);                      
  Serial.begin(9600);                                
  Wifi.begin();
  Serial.print("WIFI Connection Results：");
  if(!Wifi.connectToAP(WIFI_SSID,WIFI_PASS)) 
  {
    Serial.println("fail");
  } 
  else {Serial.println("success");}
  Serial.print("Aliyun Connection Results：");
  if(!Wifi.connectAliyunIot(CLIENTLID,USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT,PUBLISHTOPIC,SUBSCRIBERTOPIC))
  {
    Serial.println("fail");
  }
  else {Serial.println("success");}
  Serial.print("Topic set Results：");
  if(!Wifi.setTopic(CUSTOMTOPIC))
  {
    Serial.println("fail");
  }
  else {Serial.println("success");}
  delay(200);

}
void loop() 
{ 
    Wifi.readIotDataAliyun(aliyunReciveBuff,&aliyunReciveBufflen);             //监听模块接收的数据
    if(aliyunReciveBufflen)
    {
        Serial.println(aliyunReciveBufflen);
        Serial.println(aliyunReciveBuff);
    }
    while (Serial.available() > 0)
    {
      SerialBuff[resLen++] = Serial.read();
      delay(10);
    }
    if(resLen>0)
    {
      digitalWrite(LED, HIGH);
      DATA_BUF = (String )SerialBuff;
      topic = PUBLISHTOPIC;
      if(Wifi.writeStringAliyun(DATA_BUF,topic))
      {
        Serial.println("Send String data sucess");
      }
      if(Wifi.writeBytesAliyun(SerialBuff,resLen,topic))
      {
        Serial.println("Send byte data sucess");
      }
      clearBuff();
    }
}  
void clearBuff(){
  memset(SerialBuff,'\0',RES_MAX_LENGTH);
  resLen = 0;
}
