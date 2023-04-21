/*************************************************
File:             BMC81M001.h
Author:           BESTMODULES
Description:      Define classes and required variables
History：          
V1.0.1   -- initial version；2023-04-14；Arduino IDE :v1.8.13
**************************************************/

#ifndef _BMC81M001_H_
#define _BMC81M001_H_

 
#include <Arduino.h>
#include <SoftwareSerial.h>

#define BMC81M001_baudRate 115200
#define SEND_SUCCESS 1
#define SEND_FAIL 0

#define RES_MAX_LENGTH 200

/*! Subclassing Print makes debugging easier -- output en route to ESP8266 module
 * can be duplicated on a second stream (e.g. Serial). !*/
class BMC81M001 
{
  public:
      BMC81M001( HardwareSerial *theSerial = &Serial);
      BMC81M001(uint16_t rxPin,uint16_t txPin);
      void begin(uint32_t baud = BMC81M001_baudRate);  
      bool connectToAP(String ssid,String pass);
      bool connectTCP(String ip,  int port);
      bool writeDataTcp(int Dlength,char Dbuffer[]);
      String readDataTcp();
      bool connectAliyunIot(String clientlid,String username,String password,String mqtt_host,int server_port, String publishtopic,String ubscribetopic);
      bool setTopic(String topic);
      bool writeStringAliyun(String Dbuffer,String topic);
      bool writeBytesAliyun(char Dbuffer[],int Dlength,String topic);
      void readIotDataAliyun(String aliyunReciveBuff,int *aliyunReciveBufflen);
      bool reset(void);   
      
  private:
      char BMC81M001Response[RES_MAX_LENGTH];
      int resLength = 0;
      uint16_t _rxPin;
      uint16_t _txPin;
      int  sendATCommand(String StringstrCmd,int timeout,uint8_t reTry);
      void readResponse(void);
      void clearResponse(char Debugbuffer[]);
      HardwareSerial *_serial = NULL;
      SoftwareSerial *_softSerial = NULL ;

};

enum
{
  RST_Fail=1,
  Init_Fail,
  Station_Fail,
  WIFI_CONNECTED_Fail,
  TCP_CONNECTED_Fail,
  TCP_Senddata_Fail,

  MQTTSNTP_FAIL= 10,
  MQTTUSERCFG_Fail,
  MQTTCLIENTID_Fail,
  MQTTUSERNAME_Fail,
  MQTTPASSWORD_Fail,
  MQTTConnect_Fail,
  MQTTTopic_Fail
};

#endif // _BMC81M001_H_
