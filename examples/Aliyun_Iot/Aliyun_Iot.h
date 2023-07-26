#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "Aliyun_Iot.h"
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

#define WIFI_SSID "Hun"
#define WIFI_PASS "kjxxcr520"


#define CLIENTLID  "hj3hM0mUhMj.Arduino_Wifi|securemode=2\\,signmethod=hmacsha256\\,timestamp=1686301854527|"        
#define USERNAME  "Arduino_Wifi&hj3hM0mUhMj"                                           
#define PASSWORD  "ec6b343cb06f76eba5c999d04ba36df0223f3636ca9ed9ddea987622036f9168"                              
#define MQTT_HOST "iot-06z00ac1cwfkn1s.mqtt.iothub.aliyuncs.com"  
#define SERVER_PORT 1883

#define PUBLISHTOPIC "hj3hM0mUhMj/Arduino_Wifi/user/update"                    
#define SUBSCRIBERTOPIC "hj3hM0mUhMj/Arduino_Wifi/user/get"
#define CUSTOMTOPIC "hj3hM0mUhMj/Arduino_Wifi/user/ardunio"  

//*********************************************//
//************* IO_Port Define ***************//
//*********************************************//
int LED = 13;                         // LED port
//*********************************************//
//************* Variable Define ***************//
//*********************************************//
#define DEB_CNT     80                //50ms
#define RES_MAX_LENGTH 200            //max buffer length

char  SerialBuff[RES_MAX_LENGTH];   //serial buffer
char  data[30];                       //key data buffer
int   resLen;                         //serial buffer use length
String aliyunReciveBuff;
int aliyunReciveBufflen;

String DATA_BUF ;                     //
String topic ;                        //MQTT_Topic

#endif
